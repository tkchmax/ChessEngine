#include <iostream>
#include "Game.h"

using namespace std;

int main()
{
    using namespace misc;
    Game game(EPlayer::BOT, EPlayer::BOT);
    game.play(10);
//    Board b;
//    b.makeMove(Move(G1, F3, KNIGHT, NO_FIGURE, SILENT, WHITE));
//    b.makeMove(Move(F1, D3, BISHOP, NO_FIGURE, SILENT, WHITE));
//    
//    b.makeMove(Move::GetWhiteShortCastlingMove());
//    b.undoMove();
//
//    ShowBits(b.GetSideBoard(WHITE));
}

