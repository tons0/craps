#include "Window.h"
#include <windows.h>
#include <wingdi.h>
#include <cstdlib>
#include <malloc.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include "../resource.h"

int Window::windowsOpened = 0;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch(uMsg){
        case WM_CLOSE:
            DestroyWindow(hWnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK WindowProcChild(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch(uMsg){
        case WM_CLOSE:
            DestroyWindow(hWnd);
        break;
        case WM_DESTROY:
            DestroyWindow(hWnd);
            return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::Window(char* windowTitle, int width, int height, bool child):m_hInstance(GetModuleHandle(nullptr)), width(width), height(height){
    std::string wop = std::to_string(windowsOpened);
    char* _char = "Window-";

    size_t length= strlen(_char) + wop.length();

    char* ccc = new char[length + 1];

    strcpy(ccc, _char);
    strcat(ccc, wop.c_str());
    std::cout << ccc << "\n";
    CLASS_NAME = ccc;
    std::cout << CLASS_NAME << "\n";

    WNDCLASS wndClass = {};
    wndClass.lpszClassName = CLASS_NAME;
    wndClass.hInstance = m_hInstance;
    wndClass.hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 256, 256, 0);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.lpfnWndProc = child ? WindowProcChild : WindowProc;
    wndClass.cbWndExtra = 0;
    wndClass.cbClsExtra = 0;
    wndClass.hbrBackground = (HBRUSH)(4);

    RegisterClass(&wndClass);

    DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_OVERLAPPED | WS_VISIBLE;

    rect.left = 250;
    rect.top = 250;
    rect.right = rect.left + width;
    rect.bottom = rect.top + height;

    AdjustWindowRect(&rect, style, false);

    m_hWnd = CreateWindowEx(0, CLASS_NAME, windowTitle, style,
                            rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
                            NULL, NULL, m_hInstance, NULL);
    ShowWindow(m_hWnd, SW_SHOW);

    hdc = GetDC(m_hWnd);

    memory = VirtualAlloc(0, width * height * 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
    bitmap.bmiHeader.biWidth = width;
    bitmap.bmiHeader.biHeight = height;
    bitmap.bmiHeader.biPlanes = 1;
    bitmap.bmiHeader.biBitCount = 32;
    bitmap.bmiHeader.biCompression = BI_RGB;

    SetFont("Arial", 24);
    windowsOpened++;
}

void Window::Update(){
    //StretchDIBits(hdc, 0, 0, width, height, 0, 0, width, height, memory, &bitmap, DIB_RGB_COLORS, SRCCOPY);

    //ReleaseDC(m_hWnd, hdc);

    if(GetCursorPos(&p)){
        ScreenToClient(m_hWnd, &p);
    }
}

int Window::GetMouseX(){
    return p.x;
}
int Window::GetMouseY(){
    return p.y;
}

Window::~Window(){
    UnregisterClass(CLASS_NAME, m_hInstance);
}

bool Window::ProcessMessages(){
    MSG msg = {};

    while(PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)){
        if(msg.message == WM_QUIT){
            return false;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}

Window::GetWidth(){ return width; }
Window::GetHeight(){ return height; }

void Window::Fill(int rect_x, int rect_y, int rect_width, int rect_height){
    int r = (color >> 16) & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = color & 0xFF;

    RECT rect = {rect_x, rect_y, rect_width + rect_x, rect_height + rect_y};
    HBRUSH redBrush = CreateSolidBrush(RGB(r, g, b));
    FillRect(hdc, &rect, redBrush);
    DeleteObject(redBrush);
}

void Window::DrawString(char* text, int x, int y){
    int r = (color >> 16) & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = color & 0xFF;

    SelectObject(hdc, hFont);
    SetTextColor(hdc, RGB(r, g, b));

    SetBkMode(hdc, TRANSPARENT);

    TextOut(hdc, x, y, text, strlen(text));
}

void Window::DisplayImage(char* imagePath, int x, int y, int width, int height) {
    HBITMAP g_hImage = (HBITMAP)LoadImage(NULL, imagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (g_hImage == NULL) {
        SetColor(0xdd0000);
        SetFont("Arial", width > 24 ? 24 : (width < 10 ? 10 : width));
        DrawString("IMG ERROR", x, y);
        return;
    }

    BITMAP bmp;
    GetObject(g_hImage, sizeof(BITMAP), &bmp);
    int srcWidth = bmp.bmWidth;
    int srcHeight = bmp.bmHeight;

    HDC hdcImage = CreateCompatibleDC(hdc);
    SelectObject(hdcImage, g_hImage);

    StretchBlt(hdc, x, y, width, height, hdcImage, 0, 0, srcWidth, srcHeight, SRCCOPY);

    DeleteDC(hdcImage);
}

void Window::SetColor(uint32_t color){
    Window::color = color;
}

void Window::SetFont(char* fontFamily, int fontSize){
    hFont = CreateFont(fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
            OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, fontFamily);
}

HWND Window::GetHWND(){ return m_hWnd; }
