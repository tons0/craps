#ifndef PLAYER_H
#define PLAYER_H

#include "Window.h"

class Player{
    public:
        static int players;

        Player(char* nome);
        ~Player();

        void RolarDados();
        void Render(Window &window);

        const char* nome;
        int dado1;
        int dado2;
        int soma;
        int ponto;

    protected:
        double money;
        int playerId;
        int renderX;
        int renderY;

    private:
};

#endif // PLAYER_H
