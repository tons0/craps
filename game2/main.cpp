#include <iostream>
#include <time.h>
#include "Window.h"
#include "Button.h"

int main(){
    srand(time(0));
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    Window* pWindow = new Window("Nova Janela", 800, 600);
    Window* pWindow2 = new Window("Nova Janela2", 800, 600, true);
    Button* btn = new Button("Teste de botão", pWindow->GetWidth() / 2 - 100, 400, 200, 50, 0x000000, 0xefefef);

    int speed = 5;
    bool running = true;
    while(running){
        if(!pWindow->ProcessMessages()){ running = false; }

        pWindow->SetColor(0xb3ccf5);
        pWindow->Fill(0, 0, pWindow->GetWidth(), pWindow->GetHeight());

        if(GetKeyState(VK_RIGHT) & 0x8000){
            btn->x += speed;
        } else if(GetKeyState(VK_LEFT) & 0x8000){
            btn->x -= speed;
        }

        if(GetKeyState(VK_UP) & 0x8000){
            btn->y -= speed;
        } else if(GetKeyState(VK_DOWN) & 0x8000){
            btn->y += speed;
        }

        btn->Render(*pWindow);

        char* imagePath = "C:\\Users\\tonso\\Downloads\\neko.bmp";
        pWindow->DisplayImage(imagePath, 0, 0, 200, 300);

        pWindow->SetColor(0x000000);
        pWindow->SetFont("Arial", 36);
        char* txt = "Hello Beto Carreiro World!";
        pWindow->DrawString(txt, 36 + 12 + ((pWindow->GetWidth() - (36 * strlen(txt)) / 2) / 2), 56);

        pWindow->SetColor(0x343434);
        txt = "Use as setas para mover o botão!";
        pWindow->DrawString(txt, 36 + 12 + ((pWindow->GetWidth() - (36 * strlen(txt)) / 2) / 2), 106);

        pWindow->Update();
        Sleep(1000/60);
    }

    delete pWindow;

    PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);
    return 0;
}
