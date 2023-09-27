#include <iostream>
#include <Window.h>
#include <mmsystem.h>
#include <vector>
#include "Window.h"
#include "Button.h"
#include "Player.h"
#include "Game.h"
#include "../resource.h"

int main(){
    //ShowWindow(GetConsoleWindow(), SW_HIDE);

    Window* pWindow = new Window("Nova Janela", 800, 600);
    Button* btn = new Button("Teste de botão", pWindow->GetWidth() / 2 - 100, 400, 200, 50, 0x000000, 0xefefef);

    pWindow->PlaySoundFile("music.wav", 20, SND_FILENAME | SND_ASYNC | SND_LOOP);

    std::vector<Player*> players = {
        new Player("Alysson"),
        new Player("Karen"),
        new Player("Tonso"),
        new Player("Luan")
    };

    int speed = 5;
    bool running = true;
    bool r = false;
    while(running){
        if(!pWindow->ProcessMessages()){ running = false; }

        pWindow->Update();

        pWindow->SetColor(0xf5ccb3);
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

        if((GetKeyState(VK_RIGHT) & 0x8000) && !r){
            r = true;
            PlaySound("defeat.wav", NULL, SND_FILENAME | SND_ASYNC);
        } else if(!(GetKeyState(VK_RIGHT) & 0x8000)){
            r = false;
        }

        if(btn->CheckClick(*pWindow)){
            std::cout << "Clicouu!!\n";
        }

        btn->Render(*pWindow);

        char* imagePath = "C:\\Users\\tonso\\Downloads\\neko.bmp";
        pWindow->DisplayImage(imagePath, 400, 0, 200, 300);

        pWindow->SetColor(0x000000);
        char* txt = "Hello Beto Carreiro World!";
        pWindow->DrawString(txt, 36 + 12 + ((pWindow->GetWidth() - (36 * strlen(txt)) / 2) / 2), 56, "Arial", 36);

        pWindow->SetColor(0x343434);
        txt = "Use as setas para mover o botão!";
        pWindow->DrawString(txt, 36 + 12 + ((pWindow->GetWidth() - (36 * strlen(txt)) / 2) / 2), 106, "Arial", 36);

        for(auto it : players){
            it->Render(*pWindow);
        }

        Sleep(1000/60);
    }

    delete pWindow;

    PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);
    return 0;
}
