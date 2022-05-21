#pragma once
#ifndef BOARD_H_
#define BOARD_H_

#include <memory>
#include <list>
#include <stack>
#include "Figure.h"

class Board
{
public:
    using Figures = std::array<std::array<std::list<std::unique_ptr<Figure>>, 6>, 2>;
    using FigureFromCoord = std::array<std::array<std::uint8_t, 64>, 2>;

    Board();

    U64 GetSideBoard(EColor color) const;
    U64 GetAttackRays(EColor color) const;
    bool IsKingAttacked(EColor color) const;

private:
    Figures figures;
    FigureFromCoord figureFromCoord;
    std::stack<std::unique_ptr<Figure>> figureCache;
    std::list<Move> madedMoves;

};

#endif

