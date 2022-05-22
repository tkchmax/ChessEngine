#pragma once
#ifndef PLAYER_H_
#define PLAYER_H_

#include "Board.h"
#include <vector>

class Player
{
public:
    Player(std::shared_ptr<Board>board, EColor color) : board(board), color(color) {}
    virtual Move ChooseMove() = 0;
    void makeMove() { board->makeMove(ChooseMove()); }
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
    int AlphaBeta(int depth, int alpha, int beta, EColor color);
    int Quies(EColor color, int alpha, int beta);
    int Evaluate(EColor color) const;
    int GetMaterialEval(EColor color) const;
    int GetStrategyEval(EColor color) const;
    int GetMobilityEval(EColor color) const;

private:
    unsigned int depth;
    std::vector<Move> bestMoves;

};

#endif //!PLAYER_H_

