#pragma once
#include <string>

enum EMoveType
{
    SILENT,
    CAPTURE,
    SHORT_CASTLING,
    LONG_CASTLING,
    TWO_SQUARE_PAWN_MOVE,
    PAWN_TO_KNIGHT,
    PAWN_TO_BISHOP,
    PAWN_TO_QUEEN
};

std::string ToString(EMoveType);
