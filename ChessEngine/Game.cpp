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
    //board->SetFEN("rnbqk2r/pppp1ppp/3b1n2/4p3/2B1P3/2N2N2/PPPP1PPP/R1BQK2R/");
    Move move;
    for(int nMove = 0; nMove < moveLimit && !board->IsGameOver(); ++nMove)
    {
        move = whitePlayer->ChooseMove();
        move.show();
        board->makeMove(move);

        move = blackPlayer->ChooseMove();
        move.show();
        board->makeMove(move);
    }

    std::cout << board->GetPGN();
}
