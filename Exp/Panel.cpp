﻿#include "Common/Precompile.h"

#include "Panel.h"

#include "Common.h"
#include "Event.h"
#include "Image.h"

#include "Common/MathUtils/Basic.h"
#include "Common/MathUtils/2D.h"

#include "UIKit/Application.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine;
using namespace d14engine::renderer;

namespace d14uikit
{
    Panel::Panel()
        :
        Panel(uikit::makeUIObject<uikit::Panel>(
            D2D1_RECT_F{}, uikit::resource_utils::g_solidColorBrush)) { }

    _D14_UIKIT_CTOR(Panel)
        :
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>())
    {
        pimpl->uiobj = uiobj;

        pimpl->uiobj->f_onRendererUpdateObject2DAfter = [this]
        (uikit::Panel* p, Renderer* rndr)
        {
            onUpdate();
            if (pcallback->onUpdate)
            {
                pcallback->onUpdate(this);
            }
        };
        pimpl->uiobj->f_onRendererDrawD2d1ObjectBefore = [this]
        (uikit::Panel* p, Renderer* rndr)
        {
            uikit::resource_utils::g_solidColorBrush->SetColor(pimpl->bkgn.color);
            uikit::resource_utils::g_solidColorBrush->SetOpacity(pimpl->bkgn.opacity);
        };
        pimpl->uiobj->f_onRendererDrawD2d1ObjectAfter = [this]
        (uikit::Panel* p, Renderer* rndr)
        {
            uikit::resource_utils::g_solidColorBrush->SetColor(pimpl->outline.color);
            uikit::resource_utils::g_solidColorBrush->SetOpacity(pimpl->outline.opacity);

            auto frame = math_utils::inner(p->absoluteRect(), pimpl->outline.width);
            D2D1_ROUNDED_RECT outlineRect = { frame, p->roundRadiusX, p->roundRadiusY };

            rndr->d2d1DeviceContext()->DrawRoundedRectangle(
                outlineRect, uikit::resource_utils::g_solidColorBrush.Get(), pimpl->outline.width);
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
        pimpl->uiobj->f_onChangeThemeStyle = [this]
        (uikit::Panel* p, const uikit::Application::ThemeStyle& style)
        {
            ThemeStyle ts =
            {
                style.name, convert(style.color)
            };
            onChangeThemeStyle(ts);
            if (pcallback->onChangeThemeStyle)
            {
                pcallback->onChangeThemeStyle(this, ts);
            }
        };
        pimpl->uiobj->f_onChangeLangLocale = [this]
        (uikit::Panel* p, WstrParam codeName)
        {
            onChangeLangLocale(codeName);
            if (pcallback->onChangeLangLocale)
            {
                pcallback->onChangeLangLocale(this, codeName);
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

    bool Panel::release()
    {
        return pimpl->uiobj->release();
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
        return convert(pimpl->uiobj->size());
    }

    void Panel::setSize(const Size& value)
    {
        pimpl->uiobj->resize(convert(value));
    }

    int Panel::width() const
    {
        return size().width;
    }

    void Panel::setWidth(int value)
    {
        setSize({ value, height() });
    }

    int Panel::height() const
    {
        return size().height;
    }

    void Panel::setHeight(int value)
    {
        setSize({ width(), value });
    }

    Point Panel::position() const
    {
        return convert(pimpl->uiobj->position());
    }

    Point Panel::absPosition() const
    {
        return convert(pimpl->uiobj->absolutePosition());
    }

    void Panel::setPosition(const Point& value)
    {
        pimpl->uiobj->move(convert(value));
    }

    int Panel::x() const
    {
        return position().x;
    }

    int Panel::absX() const
    {
        return absPosition().x;
    }

    void Panel::setX(int value)
    {
        setPosition({ value, y() });
    }

    int Panel::y() const
    {
        return position().y;
    }

    int Panel::absY() const
    {
        return absPosition().y;
    }

    void Panel::setY(int value)
    {
        setPosition({ x(), value });
    }

    Size Panel::minSize() const
    {
        return convert(pimpl->uiobj->minimalSize());
    }

    void Panel::setMinSize(const Size& value)
    {
        pimpl->uiobj->minimalWidthHint = (float)value.width;
        pimpl->uiobj->minimalHeightHint = (float)value.height;
    }

    int Panel::minWidth() const
    {
        return math_utils::round(pimpl->uiobj->minimalWidth());
    }

    void Panel::setMinWidth(int value)
    {
        pimpl->uiobj->minimalWidthHint = (float)value;
    }

    int Panel::minHeight() const
    {
        return math_utils::round(pimpl->uiobj->minimalHeight());
    }

    void Panel::setMinHeight(int value)
    {
        pimpl->uiobj->minimalHeightHint = (float)value;
    }

    Size Panel::maxSize() const
    {
        return convert(pimpl->uiobj->maximalSize());
    }

    void Panel::setMaxSize(const Size& value)
    {
        pimpl->uiobj->maximalWidthHint = (float)value.width;
        pimpl->uiobj->maximalHeightHint = (float)value.height;
    }

    int Panel::maxWidth() const
    {
        return math_utils::round(pimpl->uiobj->maximalWidth());
    }

    void Panel::setMaxWidth(int value)
    {
        pimpl->uiobj->maximalWidthHint = (float)value;
    }

    int Panel::maxHeight() const
    {
        return math_utils::round(pimpl->uiobj->maximalHeight());
    }

    void Panel::setMaxHeight(int value)
    {
        pimpl->uiobj->maximalHeightHint = (float)value;
    }

    Color Panel::color() const
    {
        return convert(pimpl->bkgn.color);
    }

    void Panel::setColor(const Color& value)
    {
        pimpl->bkgn.color = convert(value);
    }

    float Panel::opacity() const
    {
        return pimpl->bkgn.opacity;
    }

    void Panel::setOpacity(float value)
    {
        pimpl->bkgn.opacity = value;
    }

    int Panel::outlineWidth() const
    {
        return math_utils::round(pimpl->outline.width);
    }

    void Panel::setOutlineWidth(int value)
    {
        pimpl->outline.width = (float)value;
    }

    Color Panel::outlineColor() const
    {
        return convert(pimpl->outline.color);
    }

    void Panel::setOutlineColor(const Color& value)
    {
        pimpl->outline.color = convert(value);
    }

    float Panel::outlineOpacity() const
    {
        return pimpl->outline.opacity;
    }

    void Panel::setOutlineOpacity(float value)
    {
        pimpl->outline.opacity = value;
    }

    bool Panel::animState() const
    {
        return pimpl->animState;
    }

    void Panel::setAnimState(bool value)
    {
        if (pimpl->animState != value)
        {
            if (value)
            {
                pimpl->uiobj->increaseAnimationCount();
            }
            else pimpl->uiobj->decreaseAnimationCount();
        }
        pimpl->animState = value;
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

    float Panel::bitmapOpacity() const
    {
        return pimpl->uiobj->bitmapProperty.opacity;
    }

    void Panel::setBitmapOpacity(float value)
    {
        pimpl->uiobj->bitmapProperty.opacity = value;
    }

    BitmapInterpMode Panel::bitmapInterpMode() const
    {
        return pimpl->bitmapInterpMode;
    }

    void Panel::setBitmapInterpMode(BitmapInterpMode mode)
    {
        pimpl->uiobj->bitmapProperty.interpolationMode = (D2D1_INTERPOLATION_MODE)mode;
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

    void Panel::setPinned(bool value)
    {
        if (value)
        {
            pimpl->uiobj->pinApplicationEvents();
        }
        else // unpin
        {
            pimpl->uiobj->unpinApplicationEvents();
        }
    }

    void Panel::setFocused(bool value)
    {
        auto app = uikit::Application::g_app;
        if (value)
        {
            app->focusUIObject(pimpl->uiobj);
        }
        else if (pimpl->uiobj->isFocused())
        {
            app->focusUIObject(nullptr);
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

    void Panel::addChild(Panel* uiobj)
    {
        if (uiobj != nullptr)
        {
            pimpl->uiobj->addUIObject(uiobj->pimpl->uiobj);
        }
    }

    void Panel::removeChild(Panel* uiobj)
    {
        if (uiobj != nullptr)
        {
            pimpl->uiobj->removeUIObject(uiobj->pimpl->uiobj);
        }
    }

    void Panel::pinChild(Panel* uiobj)
    {
        if (uiobj != nullptr)
        {
            pimpl->uiobj->pinUIObject(uiobj->pimpl->uiobj);
        }
    }

    void Panel::unpinChild(Panel* uiobj)
    {
        if (uiobj != nullptr)
        {
            pimpl->uiobj->unpinUIObject(uiobj->pimpl->uiobj);
        }
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

    void Panel::onUpdate() { }

    void Panel::onSize(SizeEvent* event) { }

    void Panel::onMove(MoveEvent* event) { }

    void Panel::onChangeThemeStyle(const ThemeStyle& style) { }

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
