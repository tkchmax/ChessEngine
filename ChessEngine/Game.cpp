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
    //board->SetFEN("r6k/pppbq2p/8/3p1r2/4p3/4Q1R1/PPn3PP/RN4K1 w");
    for(int nMove = 0; nMove < moveLimit && !board->IsGameOver(); ++nMove)
    {
        board->makeMove(whitePlayer->ChooseMove());
        board->makeMove(blackPlayer->ChooseMove());
    }

    std::cout << board->GetPGN();
}
