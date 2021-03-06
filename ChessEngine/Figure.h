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
    static std::unique_ptr<Figure> Create(EFigure figureName, EColor color, ESquare square);

    virtual U64 GetMoves(const U64& blockers, const U64& opposite) const = 0;
    U64 GetSilentMoves(const U64& blockers, const U64& opposite) const;
    U64 GetCaptureMoves(const U64& blockers, const U64& opposite) const;

    EColor GetColor() { return color; }
    EFigure GetFigureName() { return figureName; }
    ESquare GetSquare() { return square; }
    int GetMovesCount() { return squaresCache.size(); }

    void move(ESquare newSquare);
    void moveBack();

protected:
    Figure(EColor color, EFigure figureName, ESquare square) : 
        color(color),
        figureName(figureName),
        square(square)
    {}

protected:
    EColor color;
    EFigure figureName;
    ESquare square;
    std::stack<ESquare> squaresCache;
};

class Pawn : public Figure
{
public:
    Pawn(EColor color, ESquare square) :
        Figure(color, EFigure::PAWN, square), whitePawn(bitboards::startpos::WHITE_PAWN_START_POSITION_BITBOARD, bitboards::NOT_A_FILE, bitboards::NOT_H_FILE, NORTH, 8,7,9),
        blackPawn(bitboards::startpos::BLACK_PAWN_START_POSITION_BITBOARD, bitboards::NOT_H_FILE, bitboards::NOT_A_FILE, SOUTH, -8, -7, -9) {}
    virtual U64 GetMoves(const U64& blockers, const U64& opposite) const override;

private:
    struct Properties
    {
        Properties(U64 startpos, U64 not_lsh_file, U64 not_rsh_file, EDirection dir, int forward, int leftHook, int rightHook) :
            startpos(startpos),
            not_lsh_file(not_lsh_file),
            not_rsh_file(not_rsh_file),
            dir(dir),
            forward(forward),
            leftHook(leftHook),
            rightHook(rightHook) {}
        const U64 startpos;
        const U64 not_lsh_file;
        const U64 not_rsh_file;
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
    Knight(EColor color, ESquare square) :
        Figure(color, EFigure::KNIGHT, square) {}
    virtual U64 GetMoves(const U64& blockers, const U64& opposite) const override;
};

class Bishop : public Figure
{
public:
    Bishop(EColor color, ESquare square) : 
        Figure(color, EFigure::BISHOP, square) {}
    virtual U64 GetMoves(const U64& blockers, const U64& opposite) const override;
};

class Rook : public Figure
{
public:
    Rook(EColor color, ESquare square) :
        Figure(color, EFigure::ROOK, square) {}
    virtual U64 GetMoves(const U64& blockers, const U64& opposite) const override;
};

class Queen : public Figure
{
public:
    Queen(EColor color, ESquare square) :
        Figure(color, EFigure::QUEEN, square) {}
    virtual U64 GetMoves(const U64& blockers, const U64& opposite) const override;
};

class King : public Figure
{
public:
    King(EColor color, ESquare square) :
        Figure(color, EFigure::KING, square) {}
    virtual U64 GetMoves(const U64& blockers, const U64& opposite) const override;
};

#endif // !FIGURE_H_

