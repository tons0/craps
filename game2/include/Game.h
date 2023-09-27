#ifndef GAME_H
#define GAME_H
#include "Window.h"

class Game{
    public:
        Game(int frameRate);
        ~Game();

        void Start();
        void Stop();

    protected:
        bool running = false;
        Window* window;
        int frameRate;

    private:
};

#endif // GAME_H
