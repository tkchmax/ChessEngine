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
    //board->SetFEN("rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq");

    Move move;
    for(int nMove = 0; nMove < moveLimit && !board->IsGameOver(); ++nMove)
    {
        board->makeMove(whitePlayer->ChooseMove());
        board->makeMove(blackPlayer->ChooseMove());
    }

    std::cout << board->GetPGN();
}
