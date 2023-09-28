#include "Game.h"
#include "Window.h"
#include "Player.h"
#include "Button.h"
#include <vector>
#include <string>
#include <iostream>

Game::Game(int frameRate){
    std::vector<std::string> jogadores;

    int aws = 0;

    while(aws != 2 || jogadores.size() == 0 && jogadores.size() < 4){
        std::cout << "Deseja adicionar um jogador?\n\n1.Sim\n2.Nao\nR: ";
        std::cin >> aws;

        if(aws == 1){
            std::string novo_jogador;
            std::cout << "\nDigite o nome do novo jogador: ";
            std::cin >> novo_jogador;

            jogadores.push_back(novo_jogador);

            system("cls");
        } else if(aws == 2 && jogadores.size() == 0){
            system("cls");
            std::cout << "Eh necessario adicionar pelo menos um jogador para jogar.\n\n";
        }
    }

    if(jogadores.size() == 1){
        players = { new Player((char *)jogadores[0].c_str()) };
    } else if(jogadores.size() == 2){
        players = { new Player((char *)jogadores[0].c_str()), new Player((char *)jogadores[1].c_str()) };
    } else if(jogadores.size() == 3){
        players = { new Player((char *)jogadores[0].c_str()), new Player((char *)jogadores[1].c_str()), new Player((char *)jogadores[2].c_str()) };
    } else if(jogadores.size() == 4){
        players = { new Player((char *)jogadores[0].c_str()), new Player((char *)jogadores[1].c_str()), new Player((char *)jogadores[2].c_str()), new Player((char *)jogadores[3].c_str()) };
    }

    window = new Window("Craps", 800, 600);

    if(frameRate < 1){
        MessageBox(window->GetHWND(), "FrameRate menor ou igual que zero!", "Error", MB_ICONERROR);
        return;
    }

    this->frameRate = frameRate;
    Start();
}

Game::~Game(){
    delete this;
}

void Game::Start(){
    running = true;

    //ShowWindow(GetConsoleWindow(), SW_HIDE);

    DrawGame();

    while(running){
        Update();
    }

    delete window;
}
void Game::Stop(){
    running = false;

    delete window;
    delete this;
}

void Game::Update(){
    if(!window->ProcessMessages()){ running = false; }

    if((GetKeyState(VK_F5) & 0x8000) && !r){
        std::cout << "Soma: " << players[0]->soma << "\nRet: " << players[0]->RolarDados() << "\nPrimeira jogada: " << players[0]->primeiraJogada << "\nPonto: " << players[0]->ponto << "\n\n";
        DrawGame();
        std::cout << "Drawing!!\n";
        r = true;
    } else if(!(GetKeyState(VK_F5) & 0x8000)){
        r = false;
    }

    Sleep(1000/60);
}

void Game::DrawGame(){
    window->ClearWindow();

    Button* btn = new Button("Teste de botão", window->GetWidth() / 2 - 100, 400, 200, 50, 0x000000, 0xefefef);

    window->PlaySoundFile("music.wav", 20, SND_FILENAME | SND_ASYNC | SND_LOOP);

    int speed = 5;
    bool running = true;
    bool r = false;

    window->SetColor(0xf5ccb3);
    window->Fill(0, 0, window->GetWidth(), window->GetHeight());

    window->SetColor(0x000000);
    char* txt = "Hello Beto Carreiro World!";
    window->DrawString(txt, 36 + 12 + ((window->GetWidth() - (36 * strlen(txt)) / 2) / 2), 56, "Arial", 36);

    window->SetColor(0x343434);
    txt = "Use as setas para mover o botão!";
    window->DrawString(txt, 36 + 12 + ((window->GetWidth() - (36 * strlen(txt)) / 2) / 2), 106, "Arial", 36);

    btn->Render(*window);

    char* imagePath = "C:\\Users\\tonso\\Downloads\\neko.bmp";
    window->DisplayImage(imagePath, 400, 0, 200, 300);

    if((GetKeyState(VK_RIGHT) & 0x8000) && !r){
        r = true;
        PlaySound("defeat.wav", NULL, SND_FILENAME | SND_ASYNC);
    } else if(!(GetKeyState(VK_RIGHT) & 0x8000)){
        r = false;
    }

    if(btn->CheckClick(*window)){
        std::cout << "Clicouu!!\n";
    }

    for(auto it : players){
        it->Render(*window);
    }

    int framesToUpdate = 10;
    int f = 0;
}
