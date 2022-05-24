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
    int Evaluate(EColor color) const;
private:
    int AlphaBeta(int depth, int alpha, int beta, EColor color);
    int Quies(EColor color, int alpha, int beta);
    int GetMaterialEval(EFigure figure) const;
    int GetStrategyEval(EFigure figure) const;


private:
    unsigned int depth;
    std::vector<Move> bestMoves;    
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

