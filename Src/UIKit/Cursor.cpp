#include "Common/Precompile.h"

#include "UIKit/Cursor.h"

#include "Common/MathUtils/2D.h"
#include "Common/RuntimeError.h"

#include "UIKit/Application.h"
#include "UIKit/BitmapUtils.h"
#include "UIKit/FileSystemUtils.h"

using namespace d14engine::renderer;
using namespace fanim_literals;

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

    void Cursor::registerDrawObjects()
    {
        THROW_IF_NULL(Application::g_app);

        auto& uiCmdLayer = Application::g_app->uiCmdLayer();
        if (std::holds_alternative<Renderer::CommandLayer::D2D1Target>(uiCmdLayer->drawTarget))
        {
            auto& drawobjs2d = std::get<Renderer::CommandLayer::D2D1Target>(uiCmdLayer->drawTarget);
            drawobjs2d.insert(shared_from_this());
        }
        // No need to update the topmost draw-object priority
        // since the cursor will always be displayed at the top.
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
        THROW_IF_NULL(Application::g_app);

        IconSeries icons = {};

        // Load static icons.

#define DEF_STATIC_ICON(Name, ...) { StaticIconIndex::Name, L#Name, __VA_ARGS__ }

        std::tuple<StaticIconIndex, Wstring, D2D1_POINT_2F> staticIconNames[] =
        {
            DEF_STATIC_ICON(Alternate, { 44.0f, 0.0f }),
            DEF_STATIC_ICON(Arrow,     { 4.0f, 0.0f }),
            DEF_STATIC_ICON(BackDiag,  { 24.0f, 24.0f }),
            DEF_STATIC_ICON(Hand,      { 4.0f, 4.0f }),
            DEF_STATIC_ICON(Help,      { 4.0f, 4.0f }),
            DEF_STATIC_ICON(HorzSize,  { 24.0f, 24.0f }),
            DEF_STATIC_ICON(MainDiag,  { 24.0f, 24.0f }),
            DEF_STATIC_ICON(Move,      { 4.0f, 0.0f }),
            DEF_STATIC_ICON(Person,    { 4.0f, 0.0f }),
            DEF_STATIC_ICON(Pin,       { 4.0f, 0.0f }),
            DEF_STATIC_ICON(Select,    { 24.0f, 24.0f }),
            DEF_STATIC_ICON(Stop,      { 4.0f, 0.0f }),
            DEF_STATIC_ICON(Text,      { 24.0f, 24.0f }),
            DEF_STATIC_ICON(VertSize,  { 24.0f, 24.0f })
        };

#undef DEF_STATIC_ICON

        for (auto& name : staticIconNames)
        {
            icons.staticIcons[(size_t)std::get<0>(name)] =
            {
                bitmap_utils::loadPackedBitmap(themeName + std::get<1>(name)),
                std::get<2>(name) // hot spot offset
            };
        }

        // Load dynamic icons.

#define LOAD_DYNAMIC_ICON(Name, ...) do \
{ \
    auto frames = loadBasicIconFrames(themeName + L#Name, 16); \
    frames.hotSpotOffset = __VA_ARGS__; \
    icons.dynamicIcons[(size_t)DynamicIconIndex::Name] = std::move(frames); \
} while (0)

        LOAD_DYNAMIC_ICON(Busy,    { 16.0f, 16.0f });
        LOAD_DYNAMIC_ICON(Working, { 4.0f, 0.0f });

#undef LOAD_DYNAMIC_ICON

        return icons;
    }

    Cursor::DynamicIcon Cursor::loadBasicIconFrames(WstrParam name, size_t count)
    {
        DynamicIcon icon = {};
		
        auto& fanim = icon.bitmapData.fanim;

        fanim.frames.resize(count);
        for (size_t i = 0; i < count; ++i)
        {
            fanim.frames[i] = bitmap_utils::loadPackedBitmap(name + std::to_wstring(i + 1));
        }
        fanim.timeSpanDataInSecs = 2_jf;

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
        THROW_IF_NULL(Application::g_app);

        m_selectedIconID = index;

        if (m_iconSource == System && !m_systemIconUpdateFlag)
        {
            m_systemIconUpdateFlag = true;
            PostMessage(Application::g_app->win32Window(), WM_SETCURSOR, 0, HTCLIENT);
        }
    }

    void Cursor::setStaticIcon(WstrParam name)
    {
        m_selectedIconID.emplace<g_staticIconSeat>(name);
    }

    void Cursor::setIcon(DynamicIconIndex index)
    {
        THROW_IF_NULL(Application::g_app);

        m_selectedIconID = index;

        if (m_iconSource == System && !m_systemIconUpdateFlag)
        {
            m_systemIconUpdateFlag = true;
            PostMessage(Application::g_app->win32Window(), WM_SETCURSOR, 0, HTCLIENT);
        }
    }

    void Cursor::setDynamicIcon(WstrParam name)
    {
        m_selectedIconID.emplace<g_dynamicIconSeat>(name);
    }

    Cursor::IconSource Cursor::iconSource() const
    {
        return m_iconSource;
    }

    void Cursor::setIconSource(IconSource src)
    {
        if ((m_iconSource = src) == System)
        {
            PostMessage(Application::g_app->win32Window(), WM_SETCURSOR, 0, HTCLIENT);
        }
    }

    void Cursor::setSystemIcon()
    {
        if (m_visible)
        {
#define SET_CURSOR(Icon_Name) SetCursor(LoadCursor(nullptr, Icon_Name)); break

            if (m_selectedIconID.index() == g_staticIconSeat)
            {
                auto& iconID0 = std::get<g_staticIconSeat>(m_selectedIconID);
                if (iconID0.index() == g_basicIconSeat)
                {
                    auto& iconID = std::get<g_basicIconSeat>(iconID0);
                    switch (iconID)
                    {
                    case Alternate: SET_CURSOR(IDC_UPARROW);
                    case Arrow:     SET_CURSOR(IDC_ARROW);
                    case BackDiag:  SET_CURSOR(IDC_SIZENESW);
                    case Hand:      SET_CURSOR(IDC_HAND);
                    case Help:      SET_CURSOR(IDC_HELP);
                    case HorzSize:  SET_CURSOR(IDC_SIZEWE);
                    case MainDiag:  SET_CURSOR(IDC_SIZENWSE);
                    case Move:      SET_CURSOR(IDC_SIZEALL);
                    case Person:    SET_CURSOR(IDC_PERSON);
                    case Pin:       SET_CURSOR(IDC_PIN);
                    case Select:    SET_CURSOR(IDC_CROSS);
                    case Stop:      SET_CURSOR(IDC_NO);
                    case Text:      SET_CURSOR(IDC_IBEAM);
                    case VertSize:  SET_CURSOR(IDC_SIZENS);
                    default: SetCursor(nullptr); break;
                    }
                    return;
                }
            }
            else if (m_selectedIconID.index() == g_dynamicIconSeat)
            {
                auto& iconID0 = std::get<g_dynamicIconSeat>(m_selectedIconID);
                if (iconID0.index() == g_basicIconSeat)
                {
                    auto& iconID = std::get<g_basicIconSeat>(iconID0);
                    switch (iconID)
                    {
                    case Busy:    SET_CURSOR(IDC_WAIT);
                    case Working: SET_CURSOR(IDC_APPSTARTING);
                    default: SetCursor(nullptr); break;
                    }
                    return;
                }
            }
#undef SET_CURSOR
        }
        SetCursor(nullptr);
    }

    Cursor::StaticIcon& Cursor::getCurrentSelectedStaticIcon()
    {
        THROW_IF_NULL(Application::g_app);

        auto& iconID = std::get<g_staticIconSeat>(m_selectedIconID);
        auto& basicIcons = m_classifiedBasicIcons.at(Application::g_app->themeStyle().name);

        return (iconID.index() == g_basicIconSeat) ?
            basicIcons.staticIcons[(size_t)std::get<g_basicIconSeat>(iconID)] :
            m_customIcons.staticIcons[std::get<g_customIconSeat>(iconID)];
    }

    Cursor::DynamicIcon& Cursor::getCurrentSelectedDynamicIcon()
    {
        THROW_IF_NULL(Application::g_app);

        auto& iconID = std::get<g_dynamicIconSeat>(m_selectedIconID);
        auto& basicIcons = m_classifiedBasicIcons.at(Application::g_app->themeStyle().name);

        return (iconID.index() == g_basicIconSeat) ?
            basicIcons.dynamicIcons[(size_t)std::get<g_basicIconSeat>(iconID)] :
            m_customIcons.dynamicIcons[std::get<g_customIconSeat>(iconID)];
    }

    void Cursor::onRendererUpdateObject2DHelper(Renderer* rndr)
    {
        if (m_iconSource == UIKit)
        {
            if (m_selectedIconID.index() == g_dynamicIconSeat)
            {
                getCurrentSelectedDynamicIcon().bitmapData.update(rndr);
            }
        }
    }

    void Cursor::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        if (m_iconSource == UIKit)
        {
            if (m_lastSelectedIconID.index() == g_staticIconSeat &&
                m_selectedIconID.index() == g_dynamicIconSeat)
            {
                getCurrentSelectedDynamicIcon().bitmapData.restore();
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

                auto hs = math_utils::minus(icon.hotSpotOffset);
                auto rect = math_utils::offset(m_absoluteRect, hs);

                auto& bmpobj = icon.bitmapData;
                rndr->d2d1DeviceContext()->DrawBitmap(
                    // round to fit pixel size
                    bmpobj.data.Get(), math_utils::roundf(rect),
                    bmpobj.opacity, bmpobj.getInterpolationMode());
            }
            else if (m_selectedIconID.index() == g_dynamicIconSeat)
            {
                auto& icon = getCurrentSelectedDynamicIcon();

                auto hs = math_utils::minus(icon.hotSpotOffset);
                auto rect = math_utils::offset(m_absoluteRect, hs);

                icon.bitmapData.draw(rndr, rect);
            }
        }
    }
}
