#include "Common/Precompile.h"

#include "Event.h"
#include "Image.h"
#include "Panel.h"

#include "Common/MathUtils/Basic.h"
#include "UIKit/ColorUtils.h"
#include "UIKit/Panel.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine;
using namespace d14engine::renderer;

namespace d14uikit
{
    Panel::Panel() : Panel(Passkey{})
    {
        pimpl->uiobj = uikit::makeUIObject<uikit::Panel>(
            D2D1_RECT_F{},
            uikit::resource_utils::g_solidColorBrush);

        initialize();
    }

    Panel::Panel(Passkey)
        :
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>()) { }

    void Panel::initialize()
    {
        pimpl->uiobj->f_onRendererDrawD2d1ObjectBefore = [this]
        (uikit::Panel* p, Renderer* rndr)
        {
            uikit::resource_utils::g_solidColorBrush->SetColor(pimpl->bkgn.color);
            uikit::resource_utils::g_solidColorBrush->SetOpacity(pimpl->bkgn.opacity);
        };
        pimpl->uiobj->f_onSize = [this]
        (uikit::Panel* p, uikit::SizeEvent& e)
        {
            SizeEvent event = {};
            *event.getImpl()->uiobj = e;
            onSize(&event);
            if (pcallback->onSize)
            {
                pcallback->onSize(this, &event);
            }
        };
        pimpl->uiobj->f_onMove = [this]
        (uikit::Panel* p, uikit::MoveEvent& e)
        {
            MoveEvent event = {};
            *event.getImpl()->uiobj = e;
            onMove(&event);
            if (pcallback->onMove)
            {
                pcallback->onMove(this, &event);
            }
        };
        pimpl->uiobj->f_onChangeTheme = [this]
        (uikit::Panel* p, WstrParam themeName)
        {
            onChangeTheme(themeName);
            if (pcallback->onChangeTheme)
            {
                pcallback->onChangeTheme(this, themeName);
            }
        };
        pimpl->uiobj->f_onChangeLangLocale = [this]
        (uikit::Panel* p, WstrParam langLocaleName)
        {
            onChangeLangLocale(langLocaleName);
            if (pcallback->onChangeLangLocale)
            {
                pcallback->onChangeLangLocale(this, langLocaleName);
            }
        };
        pimpl->uiobj->f_onGetFocus = [this]
        (uikit::Panel* p)
        {
            onGetFocus();
            if (pcallback->onGetFocus)
            {
                pcallback->onGetFocus(this);
            }
        };
        pimpl->uiobj->f_onLoseFocus = [this]
        (uikit::Panel* p)
        {
            onLoseFocus();
            if (pcallback->onLoseFocus)
            {
                pcallback->onLoseFocus(this);
            }
        };
        pimpl->uiobj->f_onMouseEnter = [this]
        (uikit::Panel* p, uikit::MouseMoveEvent& e)
        {
            MouseMoveEvent event = {};
            *event.getImpl()->uiobj = e;
            onMouseEnter(&event);
            if (pcallback->onMouseEnter)
            {
                pcallback->onMouseEnter(this, &event);
            }
        };
        pimpl->uiobj->f_onMouseMove = [this]
        (uikit::Panel* p, uikit::MouseMoveEvent& e)
        {
            MouseMoveEvent event = {};
            *event.getImpl()->uiobj = e;
            onMouseMove(&event);
            if (pcallback->onMouseMove)
            {
                pcallback->onMouseMove(this, &event);
            }
        };
        pimpl->uiobj->f_onMouseLeave = [this]
        (uikit::Panel* p, uikit::MouseMoveEvent& e)
        {
            MouseMoveEvent event = {};
            *event.getImpl()->uiobj = e;
            onMouseLeave(&event);
            if (pcallback->onMouseLeave)
            {
                pcallback->onMouseLeave(this, &event);
            }
        };
        pimpl->uiobj->f_onMouseButton = [this]
        (uikit::Panel* p, uikit::MouseButtonEvent& e)
        {
            MouseButtonEvent event = {};
            *event.getImpl()->uiobj = e;
            onMouseButton(&event);
            if (pcallback->onMouseButton)
            {
                pcallback->onMouseButton(this, &event);
            }
        };
        pimpl->uiobj->f_onMouseWheel = [this]
        (uikit::Panel* p, uikit::MouseWheelEvent& e)
        {
            MouseWheelEvent event = {};
            *event.getImpl()->uiobj = e;
            onMouseWheel(&event);
            if (pcallback->onMouseWheel)
            {
                pcallback->onMouseWheel(this, &event);
            }
        };
        pimpl->uiobj->f_onKeyboard = [this]
        (uikit::Panel* p, uikit::KeyboardEvent& e)
        {
            KeyboardEvent event = {};
            *event.getImpl()->uiobj = e;
            onKeyboard(&event);
            if (pcallback->onKeyboard)
            {
                pcallback->onKeyboard(this, &event);
            }
        };
    }

    bool Panel::destory()
    {
        if (pimpl->uiobj->destroy())
        {
            pimpl->parent = nullptr;
            return true;
        }
        else return false;
    }

    bool Panel::visible() const
    {
        return pimpl->uiobj->visible();
    }

    void Panel::setVisible(bool value)
    {
        pimpl->uiobj->setVisible(value);
    }

    bool Panel::enabled() const
    {
        return pimpl->uiobj->enabled();
    }

    void Panel::setEnabled(bool value)
    {
        pimpl->uiobj->setEnabled(value);
    }

    Size Panel::size() const
    {
        auto sz = pimpl->uiobj->size();
        return
        {
            math_utils::round(sz.width),
            math_utils::round(sz.height)
        };
    }

    void Panel::setSize(const Size& value)
    {
        pimpl->uiobj->resize((float)value.width, (float)value.height);
    }

    int Panel::width() const
    {
        return math_utils::round(pimpl->uiobj->width());
    }

    void Panel::setWidth(int value)
    {
        pimpl->uiobj->resize((float)value, pimpl->uiobj->height());
    }

    int Panel::height() const
    {
        return math_utils::round(pimpl->uiobj->height());
    }

    void Panel::setHeight(int value)
    {
        pimpl->uiobj->resize(pimpl->uiobj->width(), (float)value);
    }

    Point Panel::position() const
    {
        auto pos = pimpl->uiobj->position();
        return { math_utils::round(pos.x), math_utils::round(pos.y) };
    }

    void Panel::setPosition(const Point& value)
    {
        pimpl->uiobj->move((float)value.x, (float)value.y);
    }

    int Panel::x() const
    {
        return math_utils::round(pimpl->uiobj->position().x);
    }

    void Panel::setX(int value)
    {
        auto pos = pimpl->uiobj->position();
        pimpl->uiobj->move((float)value, pos.y);
    }

    int Panel::y() const
    {
        return math_utils::round(pimpl->uiobj->position().y);
    }

    void Panel::setY(int value)
    {
        auto pos = pimpl->uiobj->position();
        pimpl->uiobj->move(pos.x, (float)value);
    }

    Color Panel::color() const
    {
        auto rgb = uikit::color_utils::convert(pimpl->bkgn.color);
        return {  rgb.R,  rgb.G,  rgb.B  };
    }

    void Panel::setColor(const Color& value)
    {
        pimpl->bkgn.color = uikit::color_utils::convert(
            uikit::color_utils::iRGB{ value.r, value.g, value.b });
    }

    float Panel::opacity() const
    {
        return pimpl->bkgn.opacity;
    }

    void Panel::setOpacity(float value)
    {
        pimpl->bkgn.opacity = value;
    }

    Image* Panel::image() const
    {
        return pimpl->image;
    }

    void Panel::setImage(Image* image)
    {
        pimpl->image = image;
        if (image != nullptr && !image->cpuRead())
        {
            pimpl->uiobj->bitmap = image->getImpl()->bitmap;
        }
        else pimpl->uiobj->bitmap.Reset();
    }

    int Panel::roundRadius() const
    {
        return math_utils::round(pimpl->uiobj->roundRadiusX);
    }

    void Panel::setRoundRadius(int value)
    {
        pimpl->uiobj->roundRadiusX =
        pimpl->uiobj->roundRadiusY = (float)value;
    }

    void Panel::setGlobal(bool value)
    {
        if (value)
        {
            pimpl->uiobj->registerDrawObjects();
            pimpl->uiobj->registerApplicationEvents();
        }
        else // local
        {
            pimpl->uiobj->unregisterDrawObjects();
            pimpl->uiobj->unregisterApplicationEvents();
        }
    }

    Panel* Panel::parent() const
    {
        return pimpl->parent;
    }

    void Panel::setParent(Panel* uiobj)
    {
        pimpl->parent = uiobj;
        if (uiobj != nullptr)
        {
            pimpl->uiobj->setParent(uiobj->pimpl->uiobj);
        }
        else pimpl->uiobj->setParent(nullptr);
    }

    void Panel::moveTopmost()
    {
        pimpl->uiobj->moveTopmost();
    }

    void Panel::moveAbove(Panel* uiobj)
    {
        pimpl->uiobj->moveAbovePeerObject(uiobj->pimpl->uiobj.get());
    }

    void Panel::moveBelow(Panel* uiobj)
    {
        pimpl->uiobj->moveBelowPeerObject(uiobj->pimpl->uiobj.get());
    }

    Panel::Callback& Panel::callback() const { return *pcallback; }

    void Panel::onSize(SizeEvent* event) { }

    void Panel::onMove(MoveEvent* event) { }

    void Panel::onChangeTheme(const std::wstring& name) { }

    void Panel::onChangeLangLocale(const std::wstring& name) { }

    void Panel::onGetFocus() { }

    void Panel::onLoseFocus() { }

    void Panel::onMouseEnter(MouseMoveEvent* event) { }

    void Panel::onMouseMove(MouseMoveEvent* event) { }

    void Panel::onMouseLeave(MouseMoveEvent* event) { }

    void Panel::onMouseButton(MouseButtonEvent* event) { }

    void Panel::onMouseWheel(MouseWheelEvent* event) { }

    void Panel::onKeyboard(KeyboardEvent* event) { }
}
