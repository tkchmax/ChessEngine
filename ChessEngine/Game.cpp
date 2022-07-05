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
    //board->SetFEN("8/R6p/5pb1/2p1k3/2P3PP/2P1N3/p4P1K/r7 w - - 2 35");
    for(int nMove = 0; nMove < moveLimit && !board->IsGameOver(); ++nMove)
    {
        board->makeMove(whitePlayer->ChooseMove());
        board->makeMove(blackPlayer->ChooseMove());
    }

    std::cout << board->GetPGN();
}
