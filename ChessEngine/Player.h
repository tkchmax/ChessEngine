#pragma once
#ifndef PLAYER_H_
#define PLAYER_H_
#include "Constants.h"
#include <memory>
#include <vector>
#include "Position.h"

enum class EPlayer {
    CONSOLE,
    BOT
};

class Player
{
public:
    Player(const std::vector<Position>* gameHist, EColor color) : gameHist(gameHist), color(color) {}
    static std::unique_ptr<Player> Create(EColor color, EPlayer type, const std::vector<Position>* gameHist);
    virtual Move pick_move() = 0;
protected:
    const std::vector<Position>* gameHist;
    EColor color;
};

class ConsolePlayer : public Player {
public:
    ConsolePlayer(EColor color, const std::vector<Position>* gameHist) : Player(gameHist, color) {}
    virtual Move pick_move() override;
};

class Bot : public Player {
public:
    Bot(EColor color, const std::vector<Position>* gameHist, int depth) : Player(gameHist, color), depth(depth) {}
    virtual Move pick_move() override;
private:
    int depth;
};

#endif //PLAYER_H_

