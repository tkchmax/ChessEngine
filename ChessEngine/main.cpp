#include <iostream>
#include "Game.h"

using namespace std;

int main()
{
    using namespace misc;
    //Game game(EPlayer::BOT, EPlayer::CONSOLE);
    Game game(EPlayer::CONSOLE, EPlayer::BOT);
    game.play();
}

