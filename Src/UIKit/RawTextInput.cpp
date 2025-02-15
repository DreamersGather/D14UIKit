#include "Common/Precompile.h"

#include "UIKit/RawTextInput.h"

#include "Common/DirectXError.h"
#include "Common/MathUtils/Basic.h"

#include "UIKit/Application.h"
#include "UIKit/Cursor.h"
#include "UIKit/PlatformUtils.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    RawTextInput::RawTextInput(
        bool multiline,
        float roundRadius,
        const D2D1_RECT_F& rect)
        :
        LabelArea({}, rect),
        multiline(multiline)
    {
        roundRadiusX = roundRadiusY = roundRadius;

        m_takeOverChildrenDrawing = true;
        m_skipChangeChildrenThemes = true;

        m_placeholder = makeUIObject<Label>();
    }

    void RawTextInput::onInitializeFinish()
    {
        LabelArea::onInitializeFinish();

        if (multiline)
        {
            THROW_IF_FAILED(m_textLayout->
                SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR));

            THROW_IF_FAILED(m_placeholder->textLayout()->
                SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR));
        }
        setVisibleTextRect(selfCoordRect());
    }

    void RawTextInput::onTextContentOffsetChange(const D2D1_POINT_2F& offset)
    {
        onTextContentOffsetChangeHelper(offset);

        if (f_onTextContentOffsetChange) f_onTextContentOffsetChange(this, offset);
    }

    void RawTextInput::onTextContentOffsetChangeHelper(const D2D1_POINT_2F& offset)
    {
        // This method intentionally left blank.
    }

    Optional<Wstring> RawTextInput::preprocessInputStr(WstrParam in)
    {
        if (multiline) return std::nullopt;

        auto lineBreakPos = in.find_first_of(L'\n');
        if (lineBreakPos == Wstring::npos)
        {
            return std::nullopt;
        }
        // Cut off at any line break to keep single line.
        else return in.substr(0, lineBreakPos);
    }

    void RawTextInput::setText(WstrParam text)
    {
        if (text != m_text)
        {
            LabelArea::setText(text);

            onTextChange(m_text);
        }
    }

    const D2D1_RECT_F& RawTextInput::visibleTextRect() const
    {
        return m_visibleTextRect;
    }

    void RawTextInput::setVisibleTextRect(const D2D1_RECT_F& rect)
    {
        m_visibleTextRect = rect;

        float maskWidth = math_utils::width(m_visibleTextRect);
        float maskHeight = math_utils::height(m_visibleTextRect);

        THROW_IF_FAILED(m_textLayout->SetMaxWidth(maskWidth));
        THROW_IF_FAILED(m_textLayout->SetMaxHeight(maskHeight));

        m_visibleTextMask.loadBitmap(
            math_utils::round<UINT>(maskWidth),
            math_utils::round<UINT>(maskHeight));

        m_placeholder->transform(m_visibleTextRect);
    }

    const SharedPtr<Label>& RawTextInput::placeholder() const
    {
        return m_placeholder;
    }

    size_t RawTextInput::hitTestCharacterOffset(const D2D1_POINT_2F& sfpt)
    {
        return LabelArea::hitTestCharacterOffset
        ({
            sfpt.x + m_textContentOffset.x - m_visibleTextRect.left,
            sfpt.y + m_textContentOffset.y - m_visibleTextRect.top
        });
    }

    D2D1_POINT_2F RawTextInput::validateTextContentOffset(const D2D1_POINT_2F& in)
    {
        D2D1_POINT_2F out = { 0.0f, 0.0f };

        auto metrics = textMetrics();
        auto metrics_width = metrics.widthIncludingTrailingWhitespace;

        out.x = std::clamp(in.x, 0.0f, std::max(metrics_width  - metrics.layoutWidth,  0.0f));
        out.y = std::clamp(in.y, 0.0f, std::max(metrics.height - metrics.layoutHeight, 0.0f));

        return out;
    }

    const D2D1_POINT_2F& RawTextInput::textContentOffset() const
    {
        return m_textContentOffset;
    }

    void RawTextInput::setTextContentOffset(const D2D1_POINT_2F& offset)
    {
        auto validOffset = validateTextContentOffset(offset);

        if (validOffset.x != m_textContentOffset.x ||
            validOffset.y != m_textContentOffset.y)
        {
            m_textContentOffset = validOffset;
            onTextContentOffsetChange(validOffset);
        }
    }

    void RawTextInput::setTextContentOffsetDirect(const D2D1_POINT_2F& offset)
    {
        m_textContentOffset = validateTextContentOffset(offset);
    }

    void RawTextInput::setIndicatorPosition(size_t characterOffset)
    {
        LabelArea::setIndicatorPosition(characterOffset);

        auto area = math_utils::size(m_visibleTextRect);
        D2D1_POINT_2F originalOffset = m_textContentOffset;

        if (m_indicatorGeometry.first.x < originalOffset.x)
        {
            originalOffset.x = m_indicatorGeometry.first.x;
        }
        else if (m_indicatorGeometry.second.x > originalOffset.x + area.width)
        {
            originalOffset.x = m_indicatorGeometry.second.x - area.width;
        }
        if (m_indicatorGeometry.first.y < originalOffset.y)
        {
            originalOffset.y = m_indicatorGeometry.first.y;
        }
        else if (m_indicatorGeometry.second.y > originalOffset.y + area.height)
        {
            originalOffset.y = m_indicatorGeometry.second.y - area.height;
        }
        setTextContentOffset(originalOffset);
    }

    void RawTextInput::performCommandCtrlX()
    {
        auto hiliteStr = m_text.substr(m_hiliteRange.offset, m_hiliteRange.count);
        resource_utils::setClipboardText(hiliteStr);

        eraseTextFragment(m_hiliteRange);

        setIndicatorPosition(m_hiliteRange.offset);

        setHiliteRange({ 0, 0 });

        onTextChange(m_text);
    }

    void RawTextInput::performCommandCtrlV()
    {
        auto content = resource_utils::getClipboardText();

        if (content.has_value())
        {
            if (m_hiliteRange.count > 0)
            {
                eraseTextFragment(m_hiliteRange);
                insertTextFragment(content.value(), m_hiliteRange.offset);

                setIndicatorPosition(m_hiliteRange.offset + content.value().size());

                setHiliteRange({ 0, 0 });
            }
            else // Insert at the indicator.
            {
                insertTextFragment(content.value(), m_indicatorCharacterOffset);

                setIndicatorPosition(m_indicatorCharacterOffset + content.value().size());
            }
            onTextChange(m_text);
        }
    }

    void RawTextInput::changeCandidateText(WstrParam str)
    {
        if (m_hiliteRange.count > 0)
        {
            eraseTextFragment(m_hiliteRange);
            insertTextFragment(str, m_hiliteRange.offset);

            setIndicatorPosition(m_hiliteRange.offset + str.size());

            setHiliteRange({ 0, 0 });
        }
        else // Insert at the indicator.
        {
            insertTextFragment(str, m_indicatorCharacterOffset);

            setIndicatorPosition(m_indicatorCharacterOffset + str.size());
        }
        onTextChange(m_text);
    }

    void RawTextInput::onRendererDrawD2d1LayerHelper(Renderer* rndr)
    {
        // Rendering ClearType text requires an opaque background, while the
        // other modes (e.g. Grayscale) do not.  The texts will be rendered
        // to m_visibleTextMask at first, so their background must be opaque
        // to be compatible with the potential ClearType text rendering mode.
        m_visibleTextMask.color = Label::getAppearance().background.color;
        m_visibleTextMask.color.a = Label::getAppearance().background.opacity;
        
        m_visibleTextMask.beginDraw(rndr->d2d1DeviceContext());
        {
            // Placeholder
            if (m_placeholder->isD2d1ObjectVisible() && m_text.empty())
            {
                auto placeholderTrans = D2D1::Matrix3x2F::Translation
                (
                    - m_visibleTextRect.left, - m_visibleTextRect.top
                );
                rndr->d2d1DeviceContext()->SetTransform(placeholderTrans);

                m_placeholder->onRendererDrawD2d1Object(rndr);
            }
            // Text Content
            auto textContentTrans = D2D1::Matrix3x2F::Translation
            (
                - (m_absoluteRect.left + m_textContentOffset.x),
                - (m_absoluteRect.top  + m_textContentOffset.y)
            );
            rndr->d2d1DeviceContext()->SetTransform(textContentTrans);

            // The indicator is drawn above the visible text mask.
            drawHiliteRange(rndr); drawText(rndr); /* drawIndicator(rndr); */
        }
        m_visibleTextMask.endDraw(rndr->d2d1DeviceContext());
    }

    void RawTextInput::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        drawBackground(rndr);

        // Visible Text
        auto dstRect = math_utils::roundf(selfCoordToAbsolute(m_visibleTextRect));

        rndr->d2d1DeviceContext()->DrawBitmap(
            m_visibleTextMask.data.Get(), dstRect,
            m_visibleTextMask.opacity, m_visibleTextMask.getInterpolationMode());

        // Indicator(|)
        D2D1_MATRIX_3X2_F originalTrans = {};
        rndr->d2d1DeviceContext()->GetTransform(&originalTrans);

        auto indicatorTrans = D2D1::Matrix3x2F::Translation
        (
            std::round(m_visibleTextRect.left - m_textContentOffset.x),
            std::round(m_visibleTextRect.top  - m_textContentOffset.y)
        );
        rndr->d2d1DeviceContext()->SetTransform(originalTrans * indicatorTrans);
        {
            indicatorConstrainedRect = math_utils::rect
            (
                m_textContentOffset, math_utils::size(m_visibleTextRect)
            );
            drawIndicator(rndr); // Vertical Line Caret
        }
        rndr->d2d1DeviceContext()->SetTransform(originalTrans);

        drawOutline(rndr); // Outline & Bottom Line

        auto& bottomLineBkgn = getAppearance().bottomLine.background;

        resource_utils::g_solidColorBrush->SetColor(bottomLineBkgn.color);
        resource_utils::g_solidColorBrush->SetOpacity(bottomLineBkgn.opacity);

        auto point0 = math_utils::offset(math_utils::leftBottom(m_absoluteRect),
        {
            roundRadiusX, getAppearance().bottomLine.bottomOffset
        });
        auto point1 = math_utils::offset(math_utils::rightBottom(m_absoluteRect),
        {
            -roundRadiusX, getAppearance().bottomLine.bottomOffset
        });
        auto brush = resource_utils::g_solidColorBrush.Get();
        float strokeWidth = getAppearance().bottomLine.strokeWidth;

        rndr->d2d1DeviceContext()->DrawLine(point0, point1, brush, strokeWidth);
    }

    void RawTextInput::onSizeHelper(SizeEvent& e)
    {
        Panel::onSizeHelper(e);

        // The text layout should adapt the visible area instead of the box self.
    }

    void RawTextInput::onChangeThemeHelper(WstrParam themeName)
    {
        LabelArea::onChangeThemeHelper(themeName);

        if (themeName == L"Light")
        {
            m_placeholder->getAppearance().foreground.color = D2D1::ColorF{ 0x8c8c8c };
        }
        else if (themeName == L"Dark")
        {
            m_placeholder->getAppearance().foreground.color = D2D1::ColorF{ 0xa6a6a6 };
        }
        getAppearance().changeTheme(Label::getAppearance(), themeName);
    }

    void RawTextInput::onKeyboardHelper(KeyboardEvent& e)
    {
        LabelArea::onKeyboardHelper(e);

        THROW_IF_NULL(Application::g_app);

        if (isFocused() && e.state.pressed())
        {
            switch (e.vkey)
            {
            case VK_BACK:
            {
                if (editable)
                {
                    if (m_hiliteRange.count > 0) // Remove the hilite text.
                    {
                        eraseTextFragment(m_hiliteRange);

                        setIndicatorPosition(m_hiliteRange.offset);

                        setHiliteRange({ 0, 0 });

                        onTextChange(m_text);
                    }
                    else if (m_indicatorCharacterOffset > 0) // Remove single character.
                    {
                        eraseTextFragment({ m_indicatorCharacterOffset - 1, 1 });

                        setIndicatorPosition(m_indicatorCharacterOffset - 1);

                        onTextChange(m_text);
                    }
                }
                break;
            }
            case VK_RETURN:
            {
                if (multiline)
                {
                    if (editable)
                    {
                        changeCandidateText(L"\n");
                    }
                    break;
                }
                [[fallthrough]];
            }
            case VK_ESCAPE:
            {
                Application::g_app->focusUIObject(nullptr);
                return; // Prevent the indicator from blinking when deactivating.
            }
            case VK_END:
            {
                setHiliteRange({ 0, 0 });
                setIndicatorPosition(m_text.size());
                break;
            }
            case VK_HOME:
            {
                setHiliteRange({ 0, 0 });
                setIndicatorPosition(0);
                break;
            }
            case VK_LEFT:
            {
                if (m_hiliteRange.count == 0)
                {
                    auto intOffset = (int)m_indicatorCharacterOffset;
                    auto offset = (size_t)std::max(intOffset - 1, 0);
                    setIndicatorPosition(offset); // avoid underflow
                }
                else // move to hilite range leftmost
                {
                    setIndicatorPosition(m_hiliteRange.offset);

                    setHiliteRange({ 0, 0 });
                }
                break;
            }
            case VK_RIGHT:
            {
                if (m_hiliteRange.count == 0)
                {
                    setIndicatorPosition(m_indicatorCharacterOffset + 1);
                }
                else // move to hilite range rightmost
                {
                    setIndicatorPosition(m_hiliteRange.offset + m_hiliteRange.count);

                    setHiliteRange({ 0, 0 });
                }
                break;
            }
            case VK_DELETE:
            {
                if (editable)
                {
                    if (m_hiliteRange.count > 0) // Remove hilite text.
                    {
                        eraseTextFragment(m_hiliteRange);

                        setIndicatorPosition(m_hiliteRange.offset);

                        setHiliteRange({ 0, 0 });

                        onTextChange(m_text);
                    }
                    else if (m_indicatorCharacterOffset >= 0 && m_text.size() > 0)
                    {
                        eraseTextFragment({ m_indicatorCharacterOffset, 1 });

                        onTextChange(m_text);
                    }
                }
                break;
            }
            default:
            {
                if (e.CTRL())
                {
                    if (editable)
                    {
                        switch (e.vkey)
                        {
                        case 'X': performCommandCtrlX(); break;
                        case 'V': performCommandCtrlV(); break;
                        default: break;
                        }
                    }
                }
                break;
            }}
            m_showIndicator = true;
            m_indicatorBlinkElapsedSecs = 0.0f;
        }
    }

    Optional<LOGFONT> RawTextInput::getCompositionFont() const
    {
        LOGFONT font = {};
        font.lfHeight = platform_utils::scaledByDpi(math_utils::round
        (
            m_indicatorGeometry.second.y - m_indicatorGeometry.first.y
        ));
        font.lfWidth = 0; // applies the font's default aspect ratio
        font.lfEscapement = font.lfOrientation = 0;
        font.lfWeight = m_textLayout->GetFontWeight();
        font.lfItalic = font.lfUnderline = font.lfStrikeOut = FALSE;
        font.lfCharSet = DEFAULT_CHARSET;
        font.lfOutPrecision = OUT_DEFAULT_PRECIS;
        font.lfClipPrecision = CLIP_DEFAULT_PRECIS;
        font.lfQuality = CLEARTYPE_NATURAL_QUALITY;
        font.lfPitchAndFamily = DEFAULT_PITCH;
        THROW_IF_FAILED(m_textLayout->GetFontFamilyName
        (
            font.lfFaceName, _countof(font.lfFaceName)
        ));
        return font;
    }

    Optional<COMPOSITIONFORM> RawTextInput::getCompositionForm() const
    {
        auto origin = selfCoordToAbsolute(math_utils::leftTop(m_visibleTextRect));

        COMPOSITIONFORM form = {};
        form.dwStyle = CFS_POINT;
        form.ptCurrentPos = platform_utils::scaledByDpi(POINT
        {
            math_utils::round(origin.x + m_indicatorGeometry.first.x - m_textContentOffset.x),
            math_utils::round(origin.y + m_indicatorGeometry.first.y - m_textContentOffset.y)
        });
        return form;
    }

    void RawTextInput::onInputStringHelper(WstrParam str)
    {
        TextInputObject::onInputStringHelper(str);

        if (editable)
        {
            // Discards the unprintable characters (ascii-code from 0 to 32).
            if (str.size() != 1 || str[0] >= L' ') changeCandidateText(str);
        }
    }
}
