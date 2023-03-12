#include "Common/Precompile.h"

#include "UIKit/Cursor.h"

#include "Common/MathUtils/2D.h"
#include "UIKit/Application.h"
#include "UIKit/BitmapUtils.h"
#include "UIKit/FileSystemUtils.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    Cursor::Cursor(const BasicIconThemeMap& icons, const D2D1_RECT_F& rect)
        :
        Panel(rect),
        m_classifiedBasicIcons(icons)
    {
        // Keep the cursor always displayed at the top.
        ISortable<IDrawObject2D>::m_priority = INT_MAX;
    }

    Cursor::BasicIconThemeMap Cursor::loadBasicIcons()
    {
        return
        {
            { L"Light", loadBasicIconSeries(L"Light") },
            { L"Dark", loadBasicIconSeries(L"Dark") }
        };
    }

    Cursor::IconSeries Cursor::loadBasicIconSeries(WstrParam themeName)
    {
        IconSeries icons = {};

        // Load static icons.

#define PUSH_STATIC_ICON(Name, ...) { StaticIconIndex::Name, L#Name, __VA_ARGS__ }

        std::tuple<StaticIconIndex, Wstring, D2D1_POINT_2F> staticIconNames[] =
        {
            PUSH_STATIC_ICON(Alternate, { -16.0f, -4.0f }),
            PUSH_STATIC_ICON(Arrow,     { -5.0f, -10.0f }),
            PUSH_STATIC_ICON(BackDiag,  { -16.0f, -16.0f }),
            PUSH_STATIC_ICON(Beam,      { -16.0f, -15.0f }),
            PUSH_STATIC_ICON(Hand,      { -11.0f, -7.0f }),
            PUSH_STATIC_ICON(Help,      { -5.0f, -10.0f }),
            PUSH_STATIC_ICON(HorzSize,  { -16.0f, -16.0f }),
            PUSH_STATIC_ICON(MainDiag,  { -16.0f, -16.0f }),
            PUSH_STATIC_ICON(Move,      { -16.0f, -16.0f }),
            PUSH_STATIC_ICON(Pen,       { -7.0f, -8.0f }),
            PUSH_STATIC_ICON(Person,    { -9.0f, -8.0f }),
            PUSH_STATIC_ICON(Pin,       { -9.0f, -8.0f }),
            PUSH_STATIC_ICON(Select,    { -16.0f, -16.0f }),
            PUSH_STATIC_ICON(Stop,      { -16.0f, -16.0f }),
            PUSH_STATIC_ICON(VertSize,  { -16.0f, -16.0f })
        };

#undef PUSH_STATIC_ICON

        for (auto& name : staticIconNames)
        {
            icons.staticIcons[(size_t)std::get<0>(name)] =
            {
                std::get<2>(name), // display offset
                bitmap_utils::loadPackedBitmap(themeName + std::get<1>(name)) // bitmap
            };
        }

        // Load dynamic icons.

#define LOAD_DYNAMIC_ICON(Name, ...) \
( \
    icons.dynamicIcons[(size_t)DynamicIconIndex::Name] = \
    loadBasicIconFrames(themeName + L#Name, 22) \
) \
.displayOffset = __VA_ARGS__;

        LOAD_DYNAMIC_ICON(Busy,    { -16.0f, -16.0f });
        LOAD_DYNAMIC_ICON(Working, { -4.0f, -12.0f });

#undef LOAD_DYNAMIC_ICON

        return icons;
    }

    Cursor::DynamicIcon Cursor::loadBasicIconFrames(WstrParam name, size_t count)
    {
        DynamicIcon icon = {};

        icon.bitmap.frames.resize(count);
        for (size_t i = 0; i < count; ++i)
        {
            icon.bitmap.frames[i] = bitmap_utils::loadPackedBitmap(name + std::to_wstring(i + 1));
        }
        icon.bitmap.equalTimeSpanOptimization.durationInSecs = 0.05f;

        return icon;
    }

    void Cursor::registerIcon(WstrParam themeName, StaticIconIndex index, const StaticIcon& icon)
    {
        auto categoryItor = m_classifiedBasicIcons.find(themeName);
        if (categoryItor != m_classifiedBasicIcons.end())
        {
            categoryItor->second.staticIcons[(size_t)index] = icon;
        }
        else (m_classifiedBasicIcons[themeName] = {}).staticIcons[(size_t)index] = icon;
    }

    void Cursor::registerIcon(WstrParam name, const StaticIcon& icon)
    {
        m_customIcons.staticIcons[name] = icon;
    }

    void Cursor::unregisterStaticIcon(WstrParam name)
    {
        m_customIcons.staticIcons.erase(name);
    }

    void Cursor::registerIcon(WstrParam themeName, DynamicIconIndex index, const DynamicIcon& icon)
    {
        auto categoryItor = m_classifiedBasicIcons.find(themeName);
        if (categoryItor != m_classifiedBasicIcons.end())
        {
            categoryItor->second.dynamicIcons[(size_t)index] = icon;
        }
        else (m_classifiedBasicIcons[themeName] = {}).dynamicIcons[(size_t)index] = icon;
    }

    void Cursor::registerIcon(WstrParam name, const DynamicIcon& icon)
    {
        m_customIcons.dynamicIcons[name] = icon;
    }

    void Cursor::unregisterDynamicIcon(WstrParam name)
    {
        m_customIcons.dynamicIcons.erase(name);
    }

    void Cursor::setIcon(StaticIconIndex index)
    {
        m_selectedIconID = index;
    }

    void Cursor::setStaticIcon(WstrParam name)
    {
        m_selectedIconID.emplace<g_staticIconSeat>(name);
    }

    void Cursor::setIcon(DynamicIconIndex index)
    {
        m_selectedIconID = index;
    }

    void Cursor::setDynamicIcon(WstrParam name)
    {
        m_selectedIconID.emplace<g_dynamicIconSeat>(name);
    }

    Cursor::StaticIcon& Cursor::getCurrentSelectedStaticIcon()
    {
        auto& iconID = std::get<g_staticIconSeat>(m_selectedIconID);
        auto& basicIcons = m_classifiedBasicIcons.at(Application::g_app->currThemeName());

        return (iconID.index() == g_basicIconSeat) ?
            basicIcons.staticIcons[(size_t)std::get<g_basicIconSeat>(iconID)] :
            m_customIcons.staticIcons[std::get<g_customIconSeat>(iconID)];
    }

    Cursor::DynamicIcon& Cursor::getCurrentSelectedDynamicIcon()
    {
        auto& iconID = std::get<g_dynamicIconSeat>(m_selectedIconID);
        auto& basicIcons = m_classifiedBasicIcons.at(Application::g_app->currThemeName());

        return (iconID.index() == g_basicIconSeat) ?
            basicIcons.dynamicIcons[(size_t)std::get<g_basicIconSeat>(iconID)] :
            m_customIcons.dynamicIcons[std::get<g_customIconSeat>(iconID)];
    }

    void Cursor::onRendererUpdateObject2DHelper(Renderer* rndr)
    {
        if (m_selectedIconID.index() == g_dynamicIconSeat)
        {
            getCurrentSelectedDynamicIcon().bitmap.update(rndr);
        }
    }

    void Cursor::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        if (m_lastSelectedIconID.index() == g_staticIconSeat &&
            m_selectedIconID.index() == g_dynamicIconSeat)
        {
            getCurrentSelectedDynamicIcon().bitmap.restore();
            increaseAnimationCount();
        }
        else if (m_lastSelectedIconID.index() == g_dynamicIconSeat &&
                 m_selectedIconID.index() == g_staticIconSeat)
        {
            decreaseAnimationCount();
        }
        m_lastSelectedIconID = m_selectedIconID;

        if (m_selectedIconID.index() == g_staticIconSeat)
        {
            auto& icon = getCurrentSelectedStaticIcon();
            auto rect = math_utils::offset(m_absoluteRect, icon.displayOffset);

            rect = math_utils::roundf(rect);
            rndr->d2d1DeviceContext()->DrawBitmap(icon.bitmap.Get(), rect);
        }
        else if (m_selectedIconID.index() == g_dynamicIconSeat)
        {
            auto& icon = getCurrentSelectedDynamicIcon();
            auto rect = math_utils::offset(m_absoluteRect, icon.displayOffset);

            icon.bitmap.rect = math_utils::roundf(rect);
            icon.bitmap.draw(rndr);
        }
    }
}
