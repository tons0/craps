#include <iostream>
#include <Window.h>
#include <mmsystem.h>
#include <vector>
#include <string>
#include "Window.h"
#include "Button.h"
#include "Player.h"
#include "Game.h"
#include "../resource.h"

int main(){
    Game* game = new Game(60);

    PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);
    return 0;
}
