#include <iostream>
#include "Game.h"

#include "Evaluate.h"
#include "Search.h"
#include "Transposition.h"

using namespace std;

int main()
{
    using namespace misc;
    //Game game(EPlayer::BOT, EPlayer::CONSOLE);
    Game game(EPlayer::CONSOLE, EPlayer::BOT);
    game.play();
}

