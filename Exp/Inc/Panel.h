#pragma once

#include "BasicType.h"
#include "Event.h"

namespace d14uikit
{
    class Image;

    class DllExport Panel : public NonCopyable
    {
        _D14_UIKIT_PIMPL(Panel)

        Panel();

        virtual ~Panel() = default;

        bool destory();

        bool visible() const;
        void setVisible(bool value);

        bool enabled() const;
        void setEnabled(bool value);

        Size size() const;
        void setSize(const Size& value);

        int width() const;
        void setWidth(int value);

        int height() const;
        void setHeight(int value);

        Point position() const;
        Point absPosition() const;
        void setPosition(const Point& value);

        int x() const;
        int absX() const;
        void setX(int value);

        int y() const;
        int absY() const;
        void setY(int value);

        Color color() const;
        void setColor(const Color& value);

        float opacity() const;
        void setOpacity(float value);

        int outlineWidth() const;
        void setOutlineWidth(int value);

        Color outlineColor() const;
        void setOutlineColor(const Color& value);

        float outlineOpacity() const;
        void setOutlineOpacity(float value);

        Image* image() const;
        void setImage(Image* imgae);

        int roundRadius() const;
        void setRoundRadius(int value);

        void setGlobal(bool value);
        void setFocused(bool value);

        Panel* parent() const;
        void setParent(Panel* uiobj);

        void addChild(Panel* uiobj);
        void removeChild(Panel* uiobj);

        void moveTopmost();
        void moveAbove(Panel* uiobj);
        void moveBelow(Panel* uiobj);

        // The UI event objects are set as non-copyable to be compatible
        // with the pimpl idiom, and they are passed as references by the
        // callbacks in the C++ code. According to pybind11 documentation,
        // however, std::function parameters will fall back to reference
        // only when they are passed as pointers, so we have to overwrite
        // the callbacks to make the UI event objects passed as pointers.
        // Also see "Functions -> Return value policies" in pybind11 docs.

        struct Callback
        {
            std::function<void(Panel*)> onUpdate = {};

            std::function<void(Panel*, SizeEvent*)> onSize = {};

            std::function<void(Panel*, MoveEvent*)> onMove = {};

            std::function<void(Panel*, const std::wstring&)> onChangeTheme = {};

            std::function<void(Panel*, const std::wstring&)> onChangeLangLocale = {};

            std::function<void(Panel*)> onGetFocus = {};

            std::function<void(Panel*)> onLoseFocus = {};

            std::function<void(Panel*, MouseMoveEvent*)> onMouseEnter = {};

            std::function<void(Panel*, MouseMoveEvent*)> onMouseMove = {};

            std::function<void(Panel*, MouseMoveEvent*)> onMouseLeave = {};

            std::function<void(Panel*, MouseButtonEvent*)> onMouseButton = {};

            std::function<void(Panel*, MouseWheelEvent*)> onMouseWheel = {};

            std::function<void(Panel*, KeyboardEvent*)> onKeyboard = {};
        };
        Callback& callback() const;
        
    protected:
        std::unique_ptr<Callback> pcallback = {};

        virtual void onUpdate();

        virtual void onSize(SizeEvent* event);

        virtual void onMove(MoveEvent* event);

        virtual void onChangeTheme(const std::wstring& name);

        virtual void onChangeLangLocale(const std::wstring& name);

        virtual void onGetFocus();

        virtual void onLoseFocus();

        virtual void onMouseEnter(MouseMoveEvent* event);

        virtual void onMouseMove(MouseMoveEvent* event);

        virtual void onMouseLeave(MouseMoveEvent* event);

        virtual void onMouseButton(MouseButtonEvent* event);

        virtual void onMouseWheel(MouseWheelEvent* event);

        virtual void onKeyboard(KeyboardEvent* event);
    };
}
