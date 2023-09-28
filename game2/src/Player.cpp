#include "Player.h"
#include <time.h>
#include <string>
#include <stdlib.h>

int Player::players = 0;

Player::Player(const char* nome):nome(nome), dado1(0), dado2(0), soma(0), ponto(0), money(500.00){
    if(players > 3){
        delete this;
        return;
    }
    srand(time(NULL));
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

    primeiraJogada = true;
}
Player& Player::operator =(char* nome){
    Player* p = new Player(nome);
    return *p;
}

Player::~Player(){}

int Player::RolarDados(){
    this->dado1 = rand() % 6;
    this->dado2 = rand() % 6;
    this->soma = this->dado1 + this->dado2;

    int ret = 0;
    if(this->primeiraJogada){
        if(this->soma == 7 || this->soma == 11){
            ret = 0;
        } else if(this->soma == 2 || this->soma == 3 || this->soma == 12){
            ret = 2;
        } else{
            this->ponto = this->soma;
            ret = 1;
        }
    } else {
        if(this->soma == this->ponto){
            ret = 2;
            money+= 50;
        } else if(soma == 7){
            ret = 0;
        } else {
            ret = 1;
        }
    }

    this->primeiraJogada = false;

    return ret;
}

void Player::Render(Window &window){
    uint32_t colors[] = { 0x4433cc, 0x227549, 0xcc4433, 0x871345 };

    std::string wop = std::to_string(ponto);
    char* _char = "Ponto: ";

    size_t length = strlen(_char) + wop.length();

    char* ccc = new char[length + 1];

    strcpy(ccc, _char);
    strcat(ccc, wop.c_str());
    char* ponto_txt = ccc;

    std::string wop2 = std::to_string(money);
    char* _char2 = "Dinheiro: ";

    size_t length2 = strlen(_char) + wop2.length();

    char* ccc2 = new char[length2 + 1];

    strcpy(ccc2, _char);
    strcat(ccc2, wop2.c_str());
    char* money_txt = ccc2;

    int padding = 20;

    int nome_width = strlen(nome)*26/2;
    int ponto_width = strlen(ponto_txt)*26/2;
    int money_width = strlen(money_txt)*26/2;

    int width = nome_width > ponto_width ? nome_width : ponto_width;
    int height = 26*3;

    int x = renderX == -1 ? window.GetWidth() - width - padding : renderX;
    int y = renderY == -1 ? window.GetHeight() - height - padding : renderY;

    window.SetColor(0xccd0fc);
    window.Fill(x, y, width+padding, height+padding);

    window.SetColor(colors[playerId]);
    window.DrawString((char*)nome, x + 10, y + 10, "Arial", 24);

    window.SetColor(0x000000);
    window.DrawString(ponto_txt, x + 10, y + 10 + 26, "Arial", 24);
    window.DrawString(money_txt, x + 10, y + 10 + 52, "Arial", 24);
}
