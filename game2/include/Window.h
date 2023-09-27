#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include <stdint.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window{
    public:
        static int windowsOpened;

        Window(char* windowTitle, int width, int height, bool child = false);
        Window(const Window&) = delete;
        Window& operator =(const Window) = delete;
        ~Window();

        int GetWidth();
        int GetHeight();

        bool ProcessMessages();

        void SetColor(uint32_t color);
        void SetFont(char* fontFamily, int fontSize);
        void Fill(int x, int y, int d_width, int d_height);
        void DrawString(char* text, int x, int y);
        void DisplayImage(char* imagePath, int x, int y, int width, int height);

        void Update();

        int GetMouseX();
        int GetMouseY();

        HWND GetHWND();

    protected:
        HDC hdc;
        RECT rect;
        int width;
        int height;
        uint32_t color;
        HFONT hFont;

    private:
        const char* CLASS_NAME;
        HINSTANCE m_hInstance;
        POINT p;
        HWND m_hWnd;
        void* memory;
        BITMAPINFO bitmap;
        int w;
};

#endif // WINDOW_H
