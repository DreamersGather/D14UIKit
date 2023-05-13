#pragma once

#include "Common.h"

#include "BasicType.h"

namespace d14uikit
{
    class Cursor;
    class Image;

    class DllExport Application : public NonCopyable
    {
        _D14_UIKIT_PIMPL(Application)

        explicit Application(
            const std::wstring& name = L"D14UIKit",
            const std::optional<float>& dpi = std::nullopt);

        virtual ~Application();

        static Application* app();

        Cursor* cursor() const;

        int run() const;
        void exit() const;

        bool visible() const;
        void setVisible(bool value);

        bool minimized() const;
        void setMinimized(bool value);

        bool maximized() const;
        void setMaximized(bool value);

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

        bool clearType() const;
        void setClearType(bool value);

        bool textVertSmooth() const;
        void setTextVertSmooth(bool value);

        bool bmpQualityInterp() const;
        void setBmpQualityInterp(bool value);

        std::unique_ptr<Image> capture() const;
    };
}
