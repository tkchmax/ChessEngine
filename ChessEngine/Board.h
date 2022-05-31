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
    Board(const Board& other);

    const Figures& GetFigures() const { return figures; }
    FigureFromCoord GetFigureFromCoord() const { return figureFromCoord; }
    FigureIter GetFigureIter(EColor color, EFigure figure, ESquare square);
    FigureIterConst GetFigureIter(EColor color, EFigure figure, ESquare square) const;
    U64 GetSideBoard(EColor color) const;
    unsigned int GetFigureCount(EColor color, EFigure figureName) const;
    unsigned int GetSideFiguresCount(EColor color) const;
    unsigned int GetFigureSumMobility(EColor color, EFigure figureName) const;
    EGamePhase GetGamePhase() const { return gamePhase; }
    U64 GetAttackRays(EColor color) const;

    void Clear();
    void SetFEN(std::string fen);
    char FigureToChar(ESquare square) const;
    std::string GetFEN() const;
    std::string GetPGN();

    bool IsKingAttacked(EColor color) const;
    bool IsShortCastlingPossible(EColor color) const;
    bool IsLongCastlingPossible(EColor color) const;
    bool IsMoveLegal(const Move& move) const;
    bool IsGameOver() const;

    MoveList GenerateMoveList(EColor color, bool isCaptureOnly = false) const;

    void makeMove(const Move& move);
    void undoMove();

    bool IsExpandedNotationNeeded(const Move& move);

private:
    MoveList GenerateMoveList_(U64 bitboard, EFigure figureName, ESquare fromSqaure, EColor color) const;
    MoveList GenerateMoveList_(const std::unique_ptr<Figure>& figure, bool isCaptureOnly) const;

    void RemoveCapture_(const Move& move);
    void RestoreCapture_();
    bool IsCastlingPossible_(EColor color, const U64& castlingBlockers, ESquare rookSquare) const;

    void MakeShortCastling_(EColor color);
    void MakeLongCastling_(EColor color);
    void MakePawnTransformMove_(const Move& move);
    void UndoPawnTransformMove_();
    void UndoShortCastling_(EColor color);
    void UndoLongCastling_(EColor color);
    void MoveFigure_(EFigure figureName, EColor color, ESquare from, ESquare to);
    void MoveBackFigure_(EFigure figureName, EColor color, ESquare from, ESquare to);


private:
    struct CastlingRights {
        void reset() {
            K = k = Q = q = false;
        }
        bool K=true;
        bool k=true;
        bool Q=true;
        bool q=true;
    };

private:
    Figures figures;
    FigureFromCoord figureFromCoord;
    std::stack<std::unique_ptr<Figure>> captureStack;
    std::stack<std::unique_ptr<Figure>> pawnTransformStack;
    std::list<Move> madedMoves;
    CastlingRights castlingRights;
    EGamePhase gamePhase;
};

#endif

