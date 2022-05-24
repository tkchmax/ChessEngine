#pragma once
#ifndef BOARD_H_
#define BOARD_H_

#include <memory>
#include <list>
#include <stack>
#include "Figure.h"
#include "MoveList.h"
#include "EGamePhase.h"

class Board
{
public:
    using Figures = std::array<std::array<std::list<std::unique_ptr<Figure>>, 6>, 2>;
    using FigureIter = std::list<std::unique_ptr<Figure>>::iterator;
    using FigureIterConst = std::list<std::unique_ptr<Figure>>::const_iterator;
    using FigureFromCoord = std::array<std::array<std::uint8_t, 64>, 2>;

    Board();
    const Figures& GetFigures() { return figures; }
    FigureIter GetFigureIter(EColor color, EFigure figure, ESquare square);
    FigureIterConst GetFigureIter(EColor color, EFigure figure, ESquare square) const;
    U64 GetSideBoard(EColor color) const;
    unsigned int GetFigureCount(EColor color, EFigure figureName);
    unsigned int GetSideFiguresCount(EColor color);
    unsigned int GetFigureSumMobility(EColor color, EFigure figureName);
    EGamePhase GetGamePhase() { return gamePhase; }
    U64 GetAttackRays(EColor color) const;

    void Clear();
    void SetFEN(std::string fen);

    bool IsKingAttacked(EColor color) const;
    bool IsShortCastlingPossible(EColor color) const;
    bool IsLongCastlingPossible(EColor color) const;
    bool IsMoveLegal(const Move& move) const;
    bool IsGameOver() const;

    MoveList GenerateMoveList(const std::unique_ptr<Figure>& figure) const;
    MoveList GenerateMoveList(EColor color, bool isCaptureOnly = false) const;
    MoveList GenerateCaptureMoveList(const std::unique_ptr<Figure>& figure) const;

    void makeMove(const Move& move);
    void undoMove();

private:
    void RemoveCapture_(const Move& move);
    void RestoreCapture_();
    bool IsCastlingPossible_(EColor color, const U64& castlingBlockers, ESquare rookSquare) const;

    void MakeShortCastling_(EColor color);
    void MakeLongCastling_(EColor color);
    void UndoShortCastling_(EColor color);
    void UndoLongCastling_(EColor color);
    void MoveFigure_(EFigure figureName, EColor color, ESquare from, ESquare to);
    void MoveBackFigure_(EFigure figureName, EColor color, ESquare from, ESquare to);

private:
    Figures figures;
    FigureFromCoord figureFromCoord;
    std::stack<std::unique_ptr<Figure>> figureCache;
    std::list<Move> madedMoves;
    EGamePhase gamePhase;
};

#endif

