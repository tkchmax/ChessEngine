#pragma once
#ifndef PLAYER_H_
#define PLAYER_H_

#include "Board.h"
#include <vector>

enum class EPlayer
{
    BOT,
    CONSOLE,
};

class Player
{
public:
    Player(std::shared_ptr<Board>board, EColor color) : board(board), color(color) {}
    virtual Move ChooseMove() = 0;
    void makeMove() { board->makeMove(ChooseMove()); }
    static std::unique_ptr<Player> Create(EPlayer type, std::shared_ptr<Board> board, EColor color);
protected:
    std::shared_ptr<Board> board;
    EColor color;
};

class Bot : public Player
{
public:
    Bot(std::shared_ptr<Board> board, EColor color, unsigned int depth = 5) :
        Player(board, color),
        depth(depth) {}
    virtual Move ChooseMove() override;

private:
    unsigned int depth;
};

class ConsolePlayer : public Player
{
public:
    ConsolePlayer(std::shared_ptr<Board> board, EColor color) : Player(board,color) {}
    virtual Move ChooseMove() override;
private:
    std::string GetAvailibleMoves();

};

#endif //!PLAYER_H_

