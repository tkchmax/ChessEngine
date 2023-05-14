#include "Game.h"

Game::Game(EPlayer p1, EPlayer p2, std::string startpos)
{
    history.push_back(Position(startpos));
    wPlayer = Player::Create(WHITE, p1, &history);
    bPlayer = Player::Create(BLACK, p2, &history);
}

void Game::play(int movesLimit)
{
    for (int i = 0; i < movesLimit; ++i) {
        const Position& currentPos = get_current_pos();
        Move move = currentPos.side_to_move() == WHITE ? wPlayer->pick_move() : bPlayer->pick_move();
        history.push_back(currentPos.make_move(move));
    }
}
