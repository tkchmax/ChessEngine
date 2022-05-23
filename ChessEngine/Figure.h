#pragma once
#ifndef FIGURE_H_
#define FIGURE_H_

#include <memory>
#include <array>
#include <stack>
#include <list>
#include "Move.h"
//#include "EFigure.h"

class Figure
{
public:
    static std::list<std::unique_ptr<Figure>> Create(EFigure figureName, EColor color, std::initializer_list<ESquare> squares);

    virtual U64 GetMoves(const U64& blockers, const U64& opposite) const = 0;
    U64 GetSilentMoves(const U64& blockers, const U64& opposite) const;
    U64 GetCaptureMoves(const U64& blockers, const U64& opposite) const;

    EColor GetColor() { return color; }
    EFigure GetFigureName() { return figureName; }
    ESquare GetSquare() { return square; }
    int GetMovesCount() { return squaresCache.size(); }
    int GetCost() { return cost; }

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

class Pawn : public Figure
{
public:
    Pawn(EColor color, ESquare square, int cost = cost_default::PAWN) :
        Figure(color, EFigure::PAWN, square, cost), whitePawn(bitboards::startpos::WHITE_PAWN_START_POSITION_BITBOARD, NORTH, 8,7,9),
        blackPawn(bitboards::startpos::BLACK_PAWN_START_POSITION_BITBOARD, SOUTH, -8, -7, -9) {}
    virtual U64 GetMoves(const U64& blockers, const U64& opposite) const override;

private:
    struct Properties
    {
        Properties(U64 startpos, EDirection dir, int forward, int leftHook, int rightHook) :
            startpos(startpos),
            dir(dir),
            forward(forward),
            leftHook(leftHook),
            rightHook(rightHook) {}
        const U64 startpos;
        const EDirection dir;
        const int forward;
        const int leftHook;
        const int rightHook;
    };
    const Properties whitePawn;
    const Properties blackPawn;
};

class Knight : public Figure
{
public:
    Knight(EColor color, ESquare square, int cost = cost_default::KNIGHT) :
        Figure(color, EFigure::KNIGHT, square, cost) {}
    virtual U64 GetMoves(const U64& blockers, const U64& opposite) const override;
};

class Bishop : public Figure
{
public:
    Bishop(EColor color, ESquare square, int cost=cost_default::BISHOP) : 
        Figure(color, EFigure::BISHOP, square, cost) {}
    virtual U64 GetMoves(const U64& blockers, const U64& opposite) const override;
};

class Rook : public Figure
{
public:
    Rook(EColor color, ESquare square, int cost = cost_default::ROOK) :
        Figure(color, EFigure::ROOK, square, cost) {}
    virtual U64 GetMoves(const U64& blockers, const U64& opposite) const override;
};

class Queen : public Figure
{
public:
    Queen(EColor color, ESquare square, int cost = cost_default::QUEEN) :
        Figure(color, EFigure::QUEEN, square, cost) {}
    virtual U64 GetMoves(const U64& blockers, const U64& opposite) const override;
};

class King : public Figure
{
public:
    King(EColor color, ESquare square, int cost = cost_default::KING) :
        Figure(color, EFigure::KING, square, cost) {}
    virtual U64 GetMoves(const U64& blockers, const U64& opposite) const override;
};

#endif // !FIGURE_H_

