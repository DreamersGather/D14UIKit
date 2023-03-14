#pragma once

#include "Common.h"

#include "BasicTypes.h"

namespace d14uikit
{
    class Cursor;

    class DllExport Application : public _D14_UIKIT_NONCOPYABLE
    {
        _D14_UIKIT_PIMPL_DEF

        Application(
            int argc, wchar_t* argv[],
            const std::wstring& name = L"D14UIKit");

        virtual ~Application();

        static Application* app();

        int run();
        void exit();

        Size size() const;
        void setSize(const Size& value);

        int width() const;
        void setWidth(int value);

        int height() const;
        void setHeight(int value);

        Point position() const;
        void setPosition(const Point& value);

        int x() const;
        void setX(int value);

        int y() const;
        void setY(int value);

        Size minSize() const;
        void setMinSize(const Size& value);

        int minWidth() const;
        void setMinWidth(int value);

        int minHeight() const;
        void setMinHeight(int value);

        bool resizable() const;
        void setResizable(bool value);

        bool fullscreen() const;
        void setFullscreen(bool value);

        int fps() const;
        bool lowEnergy() const;
        void setLowEnergy(bool value);

        const std::wstring& themeMode() const;
        void setThemeMode(const std::wstring& name);

        Color themeColor() const;
        void setThemeColor(const Color& value);

        bool useSystemTheme() const;
        void setUseSystemTheme(bool value);

        const std::wstring& langLocale() const;
        void setLangLocale(const std::wstring& name);

        Cursor* cursor() const;

    protected:
        void initialize();
        explicit Application(Passkey);
    };
}
