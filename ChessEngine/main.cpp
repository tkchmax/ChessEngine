#include <iostream>
#include "Game.h"

using namespace std;

int main()
{
    using namespace misc;
    Game game(EPlayer::BOT, EPlayer::BOT);
    game.play(20);
}

