#include "Game.h"

Game::Game(EPlayer whitePlayerType, EPlayer blackPlayerType)
{
    board = std::make_shared<Board>();
    whitePlayer = std::move(Player::Create(whitePlayerType, board, WHITE));
    blackPlayer = std::move(Player::Create(blackPlayerType, board, BLACK));
}

void Game::play(int moveLimit)
{
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
}
