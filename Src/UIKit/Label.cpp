#include "Common/Precompile.h"

#include "UIKit/Label.h"

#include "Common/DirectXError.h"
#include "Common/MathUtils/2D.h"

#include "UIKit/Application.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    Label::Label(WstrParam text, const D2D_RECT_F& rect)
        :
        Panel(rect, resource_utils::g_solidColorBrush),
        m_text(text)
    {
        TextLayoutParams layoutParams =
        {
            /* text               */ std::nullopt,
            /* textFormat         */ resource_utils::g_textFormats.at(defaultTextFormatName).Get(),
            /* maxWidth           */ std::nullopt,
            /* maxHeight          */ std::nullopt,
            /* incrementalTabStop */ 4.0f * 96.0f / 72.0f,
            /* textAlignment      */ DWRITE_TEXT_ALIGNMENT_LEADING,
            /* paragraphAlignment */ DWRITE_PARAGRAPH_ALIGNMENT_CENTER,
            /* wordWrapping       */ DWRITE_WORD_WRAPPING_NO_WRAP
        };
        m_textLayout = getTextLayout(layoutParams);
        updateTextOverhangMetrics();
    }

    Wstring Label::defaultTextFormatName = L"Default/Normal/16";

    Optional<Wstring> Label::preprocessInputStr(WstrParam in)
    {
        return std::nullopt;
    }

    const Wstring& Label::text() const
    {
        return m_text;
    }

    void Label::setText(WstrParam text)
    {
        auto out = preprocessInputStr(text);
        if (out.has_value())
        {
            m_text = out.value();
        }
        else m_text = text;

        m_textLayout = getTextLayout();
        updateTextOverhangMetrics();
    }

    void Label::setTextFormat(IDWriteTextFormat* textFormat)
    {
        m_textLayout = getTextLayout({ .textFormat = textFormat });
        updateTextOverhangMetrics();
    }

    void Label::insertTextFragment(WstrParam fragment, size_t offset)
    {
        auto out = preprocessInputStr(fragment);
        if (out.has_value())
        {
            m_text.insert(offset, out.value().data(), out.value().size());
        }
        else m_text.insert(offset, fragment.data(), fragment.size());

        m_textLayout = getTextLayout();
        updateTextOverhangMetrics();
    }

    void Label::appendTextFragment(WstrParam fragment)
    {
        auto out = preprocessInputStr(fragment);
        if (out.has_value())
        {
            m_text.append(out.value().data(), out.value().size());
        }
        else m_text.append(fragment.data(), fragment.size());

        m_textLayout = getTextLayout();
        updateTextOverhangMetrics();
    }

    void Label::eraseTextFragment(const CharacterRange& range)
    {
        size_t validOffset = std::clamp<size_t>(range.offset, 0, m_text.size());
        size_t validCount = std::clamp<size_t>(range.count, 0, m_text.size() - validOffset);

        m_text.erase(validOffset, validCount);

        m_textLayout = getTextLayout();
        updateTextOverhangMetrics();
    }

    void Label::copyTextStyle(Label* source, OptParam<WstringView> text)
    {
        if (source == nullptr) return;

        getAppearance() = source->getAppearance();

        ComPtr<IDWriteTextFormat> textFormat;
        source->m_textLayout.As(&textFormat);

        if (text.has_value())
        {
            m_text = text.value();
        }
        TextLayoutParams layoutParams =
        {
            /* text               */ text,
            /* textFormat         */ textFormat.Get(),
            /* maxWidth           */ std::nullopt,
            /* maxHeight          */ std::nullopt,
            /* incrementalTabStop */ source->m_textLayout->GetIncrementalTabStop(),
            /* textAlignment      */ source->m_textLayout->GetTextAlignment(),
            /* paragraphAlignment */ source->m_textLayout->GetParagraphAlignment(),
            /* wordWrapping       */ source->m_textLayout->GetWordWrapping()
        };
        m_textLayout = getTextLayout(layoutParams);

#define COPY_TEXT_LAYOUT_FONT_ATTR(Property_Name) \
do { \
        auto& src = source->m_textLayout; \
        decltype(src->GetFont##Property_Name()) value; \
        if (FAILED(src->GetFont##Property_Name(0, &value))) \
        { \
            value = src->GetFont##Property_Name(); \
        } \
        m_textLayout->SetFont##Property_Name( \
            value, { 0, (UINT32)m_text.size() }); \
} while (0)

        // Copy the format of the 1st character by default.

        COPY_TEXT_LAYOUT_FONT_ATTR(Size);
        COPY_TEXT_LAYOUT_FONT_ATTR(Weight);
        COPY_TEXT_LAYOUT_FONT_ATTR(Style);
        COPY_TEXT_LAYOUT_FONT_ATTR(Stretch);

#undef COPY_TEXT_LAYOUT_FONT_ATTR

        updateTextOverhangMetrics();

        drawTextOptions = source->drawTextOptions;
    }

    IDWriteTextLayout* Label::textLayout() const { return m_textLayout.Get(); }

    DWRITE_TEXT_METRICS Label::textMetrics() const
    {
        DWRITE_TEXT_METRICS metrics;
        THROW_IF_FAILED(m_textLayout->GetMetrics(&metrics));
        return metrics;
    }

    const DWRITE_OVERHANG_METRICS& Label::textOverhangs() const
    {
        return m_textOverhangs;
    }

    void Label::updateTextOverhangMetrics()
    {
        THROW_IF_FAILED(m_textLayout->GetOverhangMetrics(&m_textOverhangs));
    }

    D2D1_SIZE_F Label::textAreaSize() const
    {
        return
        {
            m_textLayout->GetMaxWidth() +
            m_textOverhangs.left +
            m_textOverhangs.right,

            m_textLayout->GetMaxHeight() +
            m_textOverhangs.top +
            m_textOverhangs.bottom
        };
    }

    ComPtr<IDWriteTextLayout> Label::getTextLayout(const TextLayoutParams& params) const
    {
        THROW_IF_NULL(Application::g_app);

        auto string = params.text.has_value() ? params.text.value().data() : m_text.data();
        auto stringLength = (UINT32)(params.text.has_value() ? params.text.value().size() : m_text.size());

        ComPtr<IDWriteTextFormat> textFormat = params.textFormat;
        if (textFormat == nullptr)
        {
            THROW_IF_FAILED(m_textLayout.As(&textFormat));
        }
        FLOAT maxWidth = params.maxWidth.has_value() ? params.maxWidth.value() :
            ((m_textLayout != nullptr) ? m_textLayout->GetMaxWidth() : width());

        FLOAT maxHeight = params.maxHeight.has_value() ? params.maxHeight.value() :
            ((m_textLayout != nullptr) ? m_textLayout->GetMaxHeight() : height());

        ComPtr<IDWriteTextLayout> textLayout;
        THROW_IF_FAILED(Application::g_app->dx12Renderer()->dwriteFactory()->CreateTextLayout(
            string,
            stringLength,
            textFormat.Get(),
            maxWidth,
            maxHeight,
            &textLayout));

        THROW_IF_FAILED(textLayout->SetIncrementalTabStop(
            params.incrementalTabStop.has_value() ?
            params.incrementalTabStop.value() :
            m_textLayout->GetIncrementalTabStop()));

        THROW_IF_FAILED(textLayout->SetTextAlignment(
            params.textAlignment.has_value() ?
            params.textAlignment.value() :
            m_textLayout->GetTextAlignment()));

        THROW_IF_FAILED(textLayout->SetParagraphAlignment(
            params.paragraphAlignment.has_value() ?
            params.paragraphAlignment.value() :
            m_textLayout->GetParagraphAlignment()));

        THROW_IF_FAILED(textLayout->SetWordWrapping(
            params.wordWrapping.has_value() ?
            params.wordWrapping.value() :
            m_textLayout->GetWordWrapping()));

        return textLayout;
    }

    DWRITE_TEXT_METRICS Label::getTextMetrics(const TextMetricsParams& params) const
    {
        DWRITE_TEXT_METRICS metrics;
        THROW_IF_FAILED(getTextLayout(params)->GetMetrics(&metrics));
        return metrics;
    }

    Label::PointHitTestResult Label::hitTestPoint(FLOAT pointX, FLOAT pointY)
    {
        PointHitTestResult result = {};
        THROW_IF_FAILED(m_textLayout->HitTestPoint
        (
            /* pointX         */ pointX,
            /* pointY         */ pointY,
            /* isTrailingHit  */ &result.isTrailingHit,
            /* isInside       */ &result.isInside,
            /* hitTestMetrics */ &result.metrics
        ));
        return result;
    }

    Label::TextPosHitTestResult Label::hitTestTextPos(UINT32 textPosition, BOOL isTrailingHit)
    {
        TextPosHitTestResult result = {};
        THROW_IF_FAILED(m_textLayout->HitTestTextPosition
        (
            /* textPosition   */ textPosition,
            /* isTrailingHit  */ isTrailingHit,
            /* pointX         */ &result.pointX,
            /* pointY         */ &result.pointY,
            /* hitTestMetrics */ &result.metrics
        ));
        return result;
    }

    Label::TextRangeHitTestResult
    Label::hitTestTextRange(UINT32 textPosition, UINT32 textLength, FLOAT originX, FLOAT originY)
    {
        UINT32 count;
        HRESULT hr;
        hr = m_textLayout->HitTestTextRange
        (
            /* _                         */ textPosition,
            /* _                         */ textLength,
            /* _                         */ originX,
            /* _                         */ originY,
            /* hitTestMetrics            */ nullptr,
            /* maxHitTestMetricsCount    */ 0,
            /* actualHitTestMetricsCount */ &count
        );
        if (hr != E_NOT_SUFFICIENT_BUFFER)
        {
            THROW_ERROR(L"Unexpected calling result.");
        }
        TextRangeHitTestResult result = { count };
        THROW_IF_FAILED(m_textLayout->HitTestTextRange
        (
            /* _                         */ textPosition,
            /* _                         */ textLength,
            /* _                         */ originX,
            /* _                         */ originY,
            /* hitTestMetrics            */ result.metrics.data(),
            /* maxHitTestMetricsCount    */ (UINT32)result.metrics.size(),
            /* actualHitTestMetricsCount */ &count
        ));
        return result;
    }

    void Label::drawBackground(renderer::Renderer* rndr)
    {
        resource_utils::g_solidColorBrush->SetColor(getAppearance().background.color);
        resource_utils::g_solidColorBrush->SetOpacity(getAppearance().background.opacity);

        Panel::drawBackground(rndr);
    }

    void Label::drawText(renderer::Renderer* rndr)
    {
        auto& foreground = m_enabled ? getAppearance().foreground : getAppearance().secondaryForeground;

        resource_utils::g_solidColorBrush->SetColor(foreground.color);
        resource_utils::g_solidColorBrush->SetOpacity(foreground.opacity);

        auto origin = absolutePosition();
        switch (hardAlignment.horz)
        {
        case HorzAlignment::Left:
        {
            origin.x = std::round(origin.x + m_textOverhangs.left);
            break;
        }
        case HorzAlignment::Center:
        {
            origin.x = std::round(origin.x + m_textOverhangs.left + (width() - textAreaSize().width) * 0.5f);
            break;
        }
        case HorzAlignment::Right:
        {
            origin.x = std::round(origin.x - m_textOverhangs.right);
            break;
        }
        default: /* HorzAlignment::None */ break;
        }
        switch (hardAlignment.vert)
        {
        case VertAlignment::Top:
        {
            origin.y = std::round(origin.y + m_textOverhangs.top);
            break;
        }
        case VertAlignment::Center:
        {
            origin.y = std::round(origin.y + m_textOverhangs.top + (height() - textAreaSize().height) * 0.5f);
            break;
        }
        case VertAlignment::Bottom:
        {
            origin.y = std::round(origin.y - m_textOverhangs.bottom);
            break;
        }
        default: /* VertAlignment::None */ break;
        }
        rndr->d2d1DeviceContext()->DrawTextLayout(
            origin,
            m_textLayout.Get(),
            resource_utils::g_solidColorBrush.Get(),
            drawTextOptions);
    }

    void Label::drawOutline(renderer::Renderer* rndr)
    {
        resource_utils::g_solidColorBrush->SetColor(getAppearance().stroke.color);
        resource_utils::g_solidColorBrush->SetOpacity(getAppearance().stroke.opacity);

        float strokeWidth = getAppearance().stroke.width;

        auto frame = math_utils::inner(m_absoluteRect, strokeWidth);
        D2D1_ROUNDED_RECT outlineRect = { frame, roundRadiusX, roundRadiusY };

        rndr->d2d1DeviceContext()->DrawRoundedRectangle(
            outlineRect, resource_utils::g_solidColorBrush.Get(), strokeWidth);
    }

    void Label::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        drawBackground(rndr); drawText(rndr); drawOutline(rndr);
    }

    void Label::onSizeHelper(SizeEvent& e)
    {
        Panel::onSizeHelper(e);

        THROW_IF_FAILED(m_textLayout->SetMaxWidth(e.size.width));
        THROW_IF_FAILED(m_textLayout->SetMaxHeight(e.size.height));

        updateTextOverhangMetrics();
    }

    void Label::onChangeThemeStyleHelper(const ThemeStyle& style)
    {
        Panel::onChangeThemeStyleHelper(style);

        getAppearance().changeTheme(style.name);
    }
}
