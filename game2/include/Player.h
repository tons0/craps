#ifndef PLAYER_H
#define PLAYER_H

#include "Window.h"

class Player{
    public:
        static int players;

        Player(const char* nome);
        Player& operator =(char* nome);
        ~Player();

        int RolarDados(); // 0 -> perdeu; 1 -> continua; 2 -> ganhou
        void Render(Window &window);

        const char* nome;
        int dado1;
        int dado2;
        int soma;
        int ponto;
        bool primeiraJogada;

    protected:

        int money;
        int playerId;
        int renderX;
        int renderY;

    private:
};

#endif // PLAYER_H
