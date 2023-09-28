#ifndef GAME_H
#define GAME_H
#include "Window.h"
#include "Player.h"
#include <vector>

class Game{
    public:
        Game(int frameRate);
        ~Game();

        void Start();
        void DrawGame();
        void Stop();

        void Update();

    protected:
        bool r;
        std::vector<Player *> players;
        bool running = false;
        Window* window;
        int frameRate;

    private:
};

#endif // GAME_H
