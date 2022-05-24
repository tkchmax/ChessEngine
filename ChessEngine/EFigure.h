#pragma once
enum EFigure
{
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    NO_FIGURE,
    COUNT = 6
};

const char* ToString(EFigure figureName);


