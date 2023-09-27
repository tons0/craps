#include "Player.h"
#include <time.h>
#include <string>

int Player::players = 0;

Player::Player(char* nome):nome(nome), dado1(0), dado2(0), soma(0), ponto(0), money(500.00){
    if(players > 3){
        delete this;
        return;
    }
    srand(time(0));
    playerId = players;
    players++;

    if(playerId <= 1){
        renderX = 0;
    } else {
        renderX = -1;
    }

    if(playerId == 0 || playerId == 2){
        renderY = 0;
    } else {
        renderY = -1;
    }
}

Player::~Player(){}

void Player::RolarDados(){
    dado1 = rand() % 6;
    dado2 = rand() % 6;
    soma = dado1 + dado2;
}

void Player::Render(Window &window){
    uint32_t colors[] = { 0x4433cc, 0x227549, 0xcc4433, 0x871345 };

    std::string wop = std::to_string(ponto);
    char* _char = "Pontos: ";

    size_t length = strlen(_char) + wop.length();

    char* ccc = new char[length + 1];

    strcpy(ccc, _char);
    strcat(ccc, wop.c_str());
    const char* ponto_txt = ccc;

    int padding = 20;

    int nome_width = strlen(nome)*26/2;
    int ponto_width = strlen(ponto_txt)*26/2;

    int width = nome_width > ponto_width ? nome_width : ponto_width;
    int height = 26*2;

    int x = renderX == -1 ? window.GetWidth() - width - padding : renderX;
    int y = renderY == -1 ? window.GetHeight() - height - padding : renderY;

    window.SetColor(0xccd0fc);
    window.Fill(x, y, width+padding, height+padding);

    window.SetColor(colors[playerId]);
    window.DrawString((char*)nome, x + 10, y + 10, "Arial", 24);

    window.SetColor(0x000000);
    window.DrawString((char*)ponto_txt, x + 10, y + 10 + 26, "Arial", 24);
}
