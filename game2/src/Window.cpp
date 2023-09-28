#include "Window.h"
#include <windows.h>
#include <wingdi.h>
#include <cstdlib>
#include <malloc.h>
#include <stdint.h>
#include <iostream>
#include <mmsystem.h>
#include <string>
#include <vector>
#include "../resource.h"

int Window::windowsOpened = 0;

std::vector<Window::DrawObjects> objectsToDraw;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch(uMsg){
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            for (const Window::DrawObjects& obj : objectsToDraw) {
                switch (obj.type) {
                    case Window::DrawObjects::Rectangle:
                        FillRect(hdc, &obj.rect, CreateSolidBrush(obj.color));
                        break;
                    case Window::DrawObjects::Oval:
                        Ellipse(hdc, obj.rect.left, obj.rect.top, obj.rect.right, obj.rect.bottom);
                        break;
                    case Window::DrawObjects::Image:
                        if (obj.image != NULL) {
                            HDC hdcImage = CreateCompatibleDC(hdc);

                            BITMAP bmp;
                            GetObject(obj.image, sizeof(BITMAP), &bmp);
                            int srcWidth = bmp.bmWidth;
                            int srcHeight = bmp.bmHeight;

                            SelectObject(hdcImage, obj.image);

                            StretchBlt(hdc, obj.rect.left, obj.rect.top, obj.rect.right - obj.rect.left, obj.rect.bottom - obj.rect.top, hdcImage, 0, 0, srcWidth, srcHeight, SRCCOPY);
                            DeleteDC(hdcImage);
                        }
                        break;
                    case Window::DrawObjects::Text:
                        SelectObject(hdc, obj.hFont);
                        SetTextColor(hdc, obj.color);
                        SetBkMode(hdc, TRANSPARENT);
                        TextOutA(hdc, obj.rect.left, obj.rect.top, obj.text, strlen(obj.text));
                        break;
                }
            }

            EndPaint(hWnd, &ps);
            break;
        }
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
    CLASS_NAME = ccc;

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

    RECT rect;
    rect.left = 250;
    rect.top = 250;
    rect.right = rect.left + width;
    rect.bottom = rect.top + height;

    AdjustWindowRect(&rect, style, false);

    m_hWnd = CreateWindowEx(0, CLASS_NAME, windowTitle, style,
                            rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
                            NULL, NULL, m_hInstance, NULL);
    ShowWindow(m_hWnd, SW_SHOW);

    windowsOpened++;
}

void Window::Update(){
    if(GetCursorPos(&p)){
        ScreenToClient(m_hWnd, &p);
    }

    objectsToDraw.clear();
    InvalidateRect(m_hWnd, NULL, TRUE);
}

int Window::GetMouseX(){
    return p.x;
}
int Window::GetMouseY(){
    return p.y;
}

Window::~Window(){
    ReleaseDC(NULL, GetDC(m_hWnd));
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

void Window::AddDrawingObject(const DrawObjects& obj) {
    objectsToDraw.push_back(obj);
    InvalidateRect(m_hWnd, NULL, TRUE);
}

void Window::Fill(int rect_x, int rect_y, int rect_width, int rect_height){
    DrawObjects obj;
    obj.type = DrawObjects::Rectangle;
    obj.rect = { rect_x, rect_y, rect_x + rect_width, rect_y + rect_height };
    obj.color = color;
    AddDrawingObject(obj);
}

void Window::DrawString(char* text, int x, int y, char* fontFamily, int fontSize){
    DrawObjects obj;
    obj.type = DrawObjects::Text;
    obj.text = text;
    obj.rect = { x, y, 0, 0 };
    obj.color = color;
    obj.hFont = CreateFont(fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
            OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, fontFamily);
    AddDrawingObject(obj);
}

void Window::DisplayImage(char* imagePath, int x, int y, int width, int height) {
    HBITMAP g_hImage = (HBITMAP)LoadImage(NULL, imagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    DrawObjects obj;
    obj.type = DrawObjects::Image;
    obj.image = g_hImage;
    obj.rect = { x, y, x + width, y + height };
    AddDrawingObject(obj);
}

void Window::SetColor(uint32_t color){
    Window::color = color;
}

HWND Window::GetHWND(){ return m_hWnd; }

void Window::PlaySoundFile(char* soundFile, int volume, DWORD settings){
    WORD a = static_cast<WORD>( (float)volume / 100 * 65535);
    waveOutSetVolume(NULL, MAKELONG(a, a));
    PlaySound(soundFile, NULL, settings);
}

void Window::ClearWindow() {
    HDC hdc = GetDC(m_hWnd);

    RECT rect;
    GetClientRect(m_hWnd, &rect);

    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(memDC, &rect, hBrush);
    DeleteObject(hBrush);

    BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);

    SelectObject(memDC, hOldBitmap);
    DeleteObject(memBitmap);
    DeleteDC(memDC);

    ReleaseDC(m_hWnd, hdc);
}
