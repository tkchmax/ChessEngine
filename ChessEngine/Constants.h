#pragma once

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <stdint.h>
#include "ESquare.h"

using U64 = uint64_t;
using U32 = uint32_t;

#define INF 0x7FFFFFFF

namespace bitboards
{
    constexpr U64 NOT_H_FILE = ~0x8080808080808080;
    constexpr U64 NOT_A_FILE = ~0x0101010101010101;
    constexpr U64 NOT_G_FILE = ~4629771061636907072;
    constexpr U64 NOT_B_FILE = ~144680345676153346;
    constexpr U64 NOT_1_RANK = ~0xFF;
    constexpr U64 NOT_2_RANK = ~0xFF00;
    constexpr U64 NOT_8_RANK = ~0xFF00000000000000;
    constexpr U64 NOT_7_RANK = ~0xFF000000000000;


    namespace startpos
    {
        constexpr U64  WHITE_PAWN_START_POSITION_BITBOARD = 0xFF00;
        constexpr U64  BLACK_PAWN_START_POSITION_BITBOARD = 0xFF000000000000;
        constexpr U64  WHITE_KNIGHT_START_POSITION_BITBOARD = 0x42;
        constexpr U64  BLACK_KNIGHT_START_POSITION_BITBOARD = 0x4200000000000000;
        constexpr U64  WHITE_BISHOP_START_POSITION_BITBOARD = 0x24;
        constexpr U64  BLACK_BISHOP_START_POSITION_BITBOARD = 0x2400000000000000;
        constexpr U64  WHITE_ROOK_START_POSITION_BITBOARD = 0x81;
        constexpr U64  BLACK_ROOK_START_POSITION_BITBOARD = 0x8100000000000000;
        constexpr U64  WHITE_QUEEN_START_POSITION_BITBOARD = 0x8;
        constexpr U64  BLACK_QUEEN_START_POSITION_BITBOARD = 0x800000000000000;
        constexpr U64  WHITE_KING_START_POSITION_BITBOARD = 0x10;
        constexpr U64  BLACK_KING_START_POSITION_BITBOARD = 0x1000000000000000;
    }
    namespace line
    {

    }
    namespace rank
    {
        constexpr U64 ONE = 0xFF;
        constexpr U64 EIGHT = 0xFF00000000000000;
    }
}

namespace cost_default
{
    constexpr int PAWN = 100;
    constexpr int KNIGHT = 300;
    constexpr int BISHOP = 301;
    constexpr int ROOK = 500;
    constexpr int QUEEN = 1000;
    constexpr int KING = INF;
}

#endif // !CONSTANTS_H_

