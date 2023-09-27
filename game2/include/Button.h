#ifndef BUTTON_H
#define BUTTON_H

#include "Window.h"

class Button
{
    public:
        Button(char* title);
        Button(char* title, int x, int y);
        Button(char* title, int x, int y, int width, int height);
        Button(char* title, int x, int y, int width, int height, uint32_t textColor, uint32_t bgColor);
        Button(char* title, int x, int y, int width, int height, uint32_t textColor, uint32_t bgColor, char* fontFamily, int fontSize);
        void Construct(char* title, int x, int y, int width, int height, uint32_t textColor, uint32_t bgColor, char* fontFamily, int fontSize);
        void Render(Window &window);
        bool CheckClick();

        bool selectable;

        int x;
        int y;
        int width;
        int height;
        int fontSize;

        uint32_t bgColor;
        uint32_t textColor;

        int borderWidth;
        uint32_t borderColor;

        char* text;
        char* fontFamily;

    private:
        bool clicked;
};

#endif // BUTTON_H
