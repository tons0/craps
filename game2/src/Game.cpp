#include "Game.h"
#include "Window.h"

Game::Game(int frameRate){
    window = new Window("Craps", 800, 600);

    if(frameRate < 1){
        MessageBox(window->GetHWND(), "FrameRate menor ou igual que zero!", "Error", MB_ICONERROR);
        delete this;
        return;
    }
    this->frameRate = frameRate;
}

Game::~Game(){
    delete this;
}

void Game::Start(){
    running = true;
}
void Game::Stop(){
    running = false;

    delete window;
    delete this;
}
