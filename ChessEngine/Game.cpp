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
    //board->SetFEN("5r1k/p7/2b1B3/3p1P2/2p2R1p/4r1n1/7K/6R1/");

    Move move;
    for(int nMove = 0; nMove < moveLimit && !board->IsGameOver(); ++nMove)
    {
        board->makeMove(whitePlayer->ChooseMove());
        board->makeMove(blackPlayer->ChooseMove());
    }

    std::cout << board->GetPGN();
}
