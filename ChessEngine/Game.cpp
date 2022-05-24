#include "Game.h"

Game::Game(EPlayer whitePlayerType, EPlayer blackPlayerType)
{
    board = std::make_shared<Board>();
    whitePlayer = std::move(Player::Create(whitePlayerType, board, WHITE));
    blackPlayer = std::move(Player::Create(blackPlayerType, board, BLACK));
}
#include <iostream>
void Game::play(int moveLimit)
{
    //board->SetFEN("rnbqk2r/ppp2ppp/4pn2/3p4/1b1P1B2/2N2N2/PPP1PPPP/R2QKB1R/");
    board->makeMove(Move(E2, E4, PAWN, NO_FIGURE, SILENT, WHITE));
    board->makeMove(Move(E7, E5, PAWN, NO_FIGURE, SILENT, BLACK));
    Move move;
    for(int nMove = 0; nMove < moveLimit && !board->IsGameOver(); ++nMove)
    {
        board->makeMove(whitePlayer->ChooseMove());
        board->makeMove(blackPlayer->ChooseMove());
    }

    std::cout << board->GetPGN();
}
