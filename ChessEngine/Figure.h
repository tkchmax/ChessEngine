#pragma once
#ifndef FIGURE_H_
#define FIGURE_H_

#include <array>
#include <stack>
#include "Move.h"
//#include "EFigure.h"

class Figure
{
public:
    virtual U64 GetMoves(const U64& blockers, const U64& opposite) const = 0;
    void move(ESquare newSquare);
    void moveBack();

protected:
    Figure(EColor color, EFigure figureName, ESquare square, int cost) : 
        color(color),
        figureName(figureName),
        square(square),
        cost(cost)
    {}

protected:
    EColor color;
    EFigure figureName;
    ESquare square;
    int cost;
    std::stack<ESquare> squaresCache;
    //std::array<int, 64> prioritySquares;
};

class Bishop : public Figure
{
public:
    Bishop(EColor color, ESquare square, int cost=cost_default::BISHOP) : 
        Figure(color, EFigure::BISHOP, square, cost) {}
    virtual U64 GetMoves(const U64& blockers, const U64& opposite) const override;
};

#endif // !FIGURE_H_

