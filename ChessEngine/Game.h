#pragma once
#ifndef GAME_H_
#define GAME_H_

#include "Player.h"

class Game
{
public:
    Game(EPlayer whitePlayerType, EPlayer blackPlayerType);
    void play(int moveLimit = INF);

    std::shared_ptr<Board> GetBoard() { return board; }
private:
    std::shared_ptr<Board> board;
    std::unique_ptr<Player> whitePlayer;
    std::unique_ptr<Player> blackPlayer;
};

#endif // !GAME_H_


