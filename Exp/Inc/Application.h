#pragma once

#include <Windows.h>

#include "Common.h"

#include "BasicEnum.h"
#include "BasicType.h"

_D14_UIKIT_FWDEF(Application)

namespace d14uikit
{
    class Cursor;
    class Image;
    class Renderer;

    class DllExport Application : public NonCopyable
    {
        _D14_UIKIT_PIMPL(Application)

        explicit Application(
            const std::wstring& name = L"D14UIKit",
            const std::optional<float>& dpi = std::nullopt);

        virtual ~Application();

        static Application* app();

        int dpi() const;

        BitmapInterpMode bitmapInterpMode() const;
        void setBitmapInterpMode(BitmapInterpMode mode);

        int run() const;
        void exit() const;

        HWND win32Window() const;

#ifdef _WIN64
        using ThreadID = uint64_t;
        using ThreadData = uint64_t;
#else
        using ThreadID = uint32_t;
        using ThreadData = uint32_t;
#endif
        using ThreadCallback = std::function<void(ThreadData)>;
        void addThreadCallback(ThreadID id, const ThreadCallback& callback);
        void removeThreadCallback(ThreadID id);

        void setThreadEvent(ThreadID id, ThreadData data = 0);

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

        Renderer* dx12Renderer() const;

        bool fullscreen() const;
        void setFullscreen(bool value);

        int fps() const;

        // This method requires flushing the GPU command list,
        // so it cannot be called in the update routine of each frame.
        std::unique_ptr<Image> capture() const;

        TextAntialiasMode textAntialiasMode() const;
        void setTextAntialiasMode(TextAntialiasMode mode);

        RenderingMode renderingMode() const;
        void setRenderingMode(RenderingMode mode);

        // Returns the number of UI objects
        // with animState=true in the entire application
        int animCount() const;

        // The global animation state of the application,
        // which is at the same level as the animation state of UI objects.
        // Therefore, when this value is false, an animation may still be playing.
        // To determine the actual animation state, use animCount;
        // a value greater than 0 indicates that there is an animation.
        bool animState() const;
        void setAnimState(bool value);

        Cursor* cursor() const;

        const std::wstring& themeMode() const;
        void setThemeMode(const std::wstring& name);

        Color themeColor() const;
        void setThemeColor(const Color& value);

        bool useSystemTheme() const;
        void setUseSystemTheme(bool value);

        const std::wstring& langLocale() const;
        void setLangLocale(const std::wstring& name);
    };
}
