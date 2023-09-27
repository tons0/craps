#include "Button.h"

Button::Button(char* title){
    Construct(title, -2147483647, -2147483647, 70, 30, 0xffffff, 0xcccccc, "Arial", height / 3);
}

Button::Button(char* title, int x, int y){
    Construct(title, x, y, 70, 30, 0xffffff, 0xcccccc, "Arial", height / 3);
}

Button::Button(char* title, int x, int y, int width, int height){
    Construct(title, x, y, width, height, 0xffffff, 0xcccccc, "Arial", height / 3);
}

Button::Button(char* title, int x, int y, int width, int height, uint32_t textColor, uint32_t bgColor){
    Construct(title, x, y, width, height, textColor, bgColor, "Arial", height / 3);
}

Button::Button(char* title, int x, int y, int width, int height, uint32_t textColor, uint32_t bgColor, char* fontFamily, int fontSize){
    Construct(title, x, y, width, height, textColor, bgColor, fontFamily, fontSize);
}

void Button::Construct(char* title, int x, int y, int width, int height, uint32_t textColor, uint32_t bgColor, char* fontFamily, int fontSize){
    this->text = title;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->textColor = textColor;
    this->bgColor = bgColor;
    this->fontFamily = fontFamily;
    this->fontSize = fontSize;
}

void Button::Render(Window &window){
    if(x == -2147483647){
        x = (window.GetWidth() - width) / 2;
    }
    if(y == -2147483647){
        y = (window.GetHeight() - height) / 2;
    }
    window.SetColor(bgColor);
    window.Fill(x, y, width, height);

    window.SetColor(textColor);
    window.SetFont(fontFamily, fontSize);

    int fX = fontSize /2 + x + ((width - (fontSize * strlen(text)) / 2) / 2);
    int fY = fontSize + y;
    window.DrawString(text, fX, fY);
}

bool Button::CheckClick(){
    if(GetKeyState(VK_LBUTTON) & 0x800 && !clicked){
        clicked = true;
    } else if(!(GetKeyState(VK_LBUTTON) & 0x800)){
        clicked = false;
    }

    return clicked;
}
