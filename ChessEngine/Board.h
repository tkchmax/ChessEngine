#pragma once
#ifndef BOARD_H_
#define BOARD_H_

#include <memory>
#include <list>
#include <stack>
#include "Figure.h"
#include "MoveList.h"

class Board
{
public:
    using Figures = std::array<std::array<std::list<std::unique_ptr<Figure>>, 6>, 2>;
    using FigureIter = std::list<std::unique_ptr<Figure>>::iterator;
    using FigureFromCoord = std::array<std::array<std::uint8_t, 64>, 2>;

    Board();
    FigureIter GetFigureIter(EColor color, EFigure figure, ESquare square);
    U64 GetSideBoard(EColor color) const;
    U64 GetAttackRays(EColor color) const;
    bool IsKingAttacked(EColor color) const;
    bool IsMoveLegal(const Move& move) const;

    MoveList GenerateMoveList(const std::unique_ptr<Figure>& figure) const;
    MoveList GenerateMoveList(EColor color, bool isCaptureOnly = false) const;
    MoveList GenerateCaptureMoveList(const std::unique_ptr<Figure>& figure) const;

    void makeMove(const Move& move);
    void undoMove();

private:
    void RemoveCapture_(const Move& move);
    void RestoreCapture_();

private:
    Figures figures;
    FigureFromCoord figureFromCoord;
    std::stack<std::unique_ptr<Figure>> figureCache;
    std::list<Move> madedMoves;

};

#endif

