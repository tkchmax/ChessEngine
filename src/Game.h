#pragma once
#ifndef GAME_H_
#define GAME_H_
#include "Player.h"
#include <memory>

class Game
{
public:
    Game(EPlayer p1, EPlayer p2, std::string startpos= "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    void play(int movesLimit = INF);
    const Position& get_current_pos() const { return history.back(); }
    const std::vector<Position>& get_history() const { return history; }
private:
    std::unique_ptr<Player> wPlayer, bPlayer;
    std::vector<Position> history;

};

#endif //GAME_H_
