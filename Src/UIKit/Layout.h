#pragma once

#include "Common/Precompile.h"

#include "Common/MathUtils/2D.h"

#include "UIKit/Appearances/Layout.h"
#include "UIKit/ResizablePanel.h"
#include "UIKit/ResourceUtils.h"

namespace d14engine::uikit
{
    template<typename GeometryInfo_T>
    struct Layout : appearance::Layout, ResizablePanel
    {
        using LayoutType = Layout<GeometryInfo_T>;

        using GeometryInfo = GeometryInfo_T;

        explicit Layout(const D2D1_RECT_F& rect = {})
            :
            Panel(rect, resource_utils::g_solidColorBrush),
            ResizablePanel(rect, resource_utils::g_solidColorBrush)
        {
            appEventReactability.focus.get = false;

            setResizable(false);
        }
        _D14_SET_APPEARANCE_GETTER(Layout)

    protected:
        using ElementGeometryInfoMap = std::unordered_map<SharedPtr<Panel>, GeometryInfo_T>;

        ElementGeometryInfoMap m_elemGeoInfos = {};

        virtual void updateElement(ShrdPtrParam<Panel> elem, const GeometryInfo_T& geoInfo) = 0;

    public:
        void addElement(ShrdPtrParam<Panel> elem, const GeometryInfo_T& geoInfo)
        {
            if (!elem) return;
            addUIObject(elem);

            m_elemGeoInfos.insert({ elem, geoInfo });
            updateElement(elem, geoInfo);
        }

        void removeElement(ShrdPtrParam<Panel> elem)
        {
            m_elemGeoInfos.erase(elem);
            removeUIObject(elem);
        }

        void clearAllElements()
        {
            m_elemGeoInfos.clear();
            clearAddedUIObjects();
        }

        void updateElement(ElementGeometryInfoMap::iterator elemItor)
        {
            if (elemItor != m_elemGeoInfos.end()) updateElement(elemItor->first, elemItor->second);
        }

        void updateElement(ShrdPtrParam<Panel> elem)
        {
            updateElement(m_elemGeoInfos.find(elem));
        }

        void updateAllElements()
        {
            for (auto& kv : m_elemGeoInfos) updateElement(kv.first, kv.second);
        }

        Optional<typename ElementGeometryInfoMap::iterator> findElement(ShrdPtrParam<Panel> elem)
        {
            auto elemItor = m_elemGeoInfos.find(elem);
            if (elemItor != m_elemGeoInfos.end()) return elemItor;
            else return std::nullopt;
        }

    protected:
        // IDrawObject2D
        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override
        {
            // Background
            resource_utils::g_solidColorBrush->SetColor(getAppearance().background.color);
            resource_utils::g_solidColorBrush->SetOpacity(getAppearance().background.opacity);

            Panel::drawBackground(rndr);
        }

        void drawD2d1ObjectPosterior(renderer::Renderer* rndr) override
        {
            Panel::drawD2d1ObjectPosterior(rndr);

            // Outline
            resource_utils::g_solidColorBrush->SetColor(getAppearance().stroke.color);
            resource_utils::g_solidColorBrush->SetOpacity(getAppearance().stroke.opacity);

            float strokeWidth = getAppearance().stroke.width;

            auto frame = math_utils::inner(m_absoluteRect, strokeWidth);
            D2D1_ROUNDED_RECT outlineRect = { frame, roundRadiusX, roundRadiusY };

            rndr->d2d1DeviceContext()->DrawRoundedRectangle(
                outlineRect, resource_utils::g_solidColorBrush.Get(), strokeWidth);
        }

        // Panel
        bool destroyUIObjectHelper(ShrdPtrParam<Panel> uiobj) override
        {
            removeElement(uiobj);
            return true;
        }

        void onSizeHelper(SizeEvent& e) override
        {
            ResizablePanel::onSizeHelper(e);

            updateAllElements();
        }

        void onChangeThemeHelper(WstrParam themeName) override
        {
            ResizablePanel::onChangeThemeHelper(themeName);

            getAppearance().changeTheme(themeName);
        }
    };
}
