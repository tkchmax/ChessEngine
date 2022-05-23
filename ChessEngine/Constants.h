#pragma once

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <stdint.h>
#include "ESquare.h"
#include <array>

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

namespace figure_priority_squares
{
	static const std::array<int, 64> WHITE_PAWN = { 0, 0, 0, 0, 0, 0, 0, 0,
										4, 4, 4, 0, 0, 4, 4, 4,
										6, 8, 2, 10,10,2, 8, 6,
										6, 8, 12,16,16,12,8, 6,
										6, 12,16,24,24,16,12,8,
										12, 16,24,32,32,24,16,12,
										15, 20,30,35,35,30,20,15,
										0, 0, 0, 0, 0, 0, 0, 0 };

	static const std::array<int, 64> BLACK_PAWN = { 0, 0, 0, 0, 0, 0, 0, 0,
											15, 20,30,35,35,30,20,15,
											12, 16,24, 32,32,24,16,12,
											6,  12,16, 24,24,16,12, 8,
											6,	8, 12, 16,16,12, 8, 6,
											6,  8, 2,  10,10, 2, 8, 6,
											4,  4, 4,  0,  0, 4, 4, 4,
											0,  0, 0,  0,  0, 0, 0, 0 };

	static const std::array<int, 64> KNIGHT = { 0, 2, 8, 10, 10, 8, 2, 0,
										4 ,8, 16, 20, 20, 16, 8, 4,
										8, 16, 25, 28, 28, 25, 16,8,
										10, 20,28, 32, 32, 28, 20,10,
										10, 20,28, 32, 32, 28, 20,10,
										8, 16, 25, 28, 28, 25, 16, 8,
										4, 8, 16, 20, 20, 16,  8,  4,
										0, 2, 8,  10, 10  ,8  ,2  ,0 };

	static const std::array<int, 64> BISHOP = { 0,	0,	0,	0,	0,	0,	0,	0,
										5,  23,  15,  15,  15,  15,  23,5,
										5,  18,  22, 22, 22,  22,  18,5,
										5,  18,  22, 22, 22,  22,   18,5,
										5,  18, 22, 22, 22,  22,   18,5,
										5, 18,22, 22, 22,   22, 18,	5,
										5,  23,  15,  15,  15,  15,  23,5,
										0,	0,	0,	0,	0,	0,	0,	0 };

	static const std::array<int, 64> ROOK = { 0, 0, 0,  5,  5, 5, 0, 0,
										0, 0, 0,  0,  0, 0, 0, 0,
										0, 0, 0,  0,  0, 0, 0, 0,
										0, 0, 0,  0,  0, 0, 0, 0,
										0, 0, 0,  0,  0, 0, 0, 0,
										0, 0, 0,  0,  0, 0, 0, 0,
										0, 0, 0,  0,  0, 0, 0, 0,
										0, 0, 0,  5,  5, 5, 0, 0 };

	static const std::array<int, 64> QUEEN = { -20,-10,-10, 0, 0,-10,-10,-20,
										-10,  0,  0,  0,  0,  0,  0,-10,
										-10,  0,  5,  5,  5,  5,  0,-10,
										 -5,  0,  5,  5,  5,  5,  0, -5,
										 -5,  0,  5,  5,  5,  5,  0, -5,
										-10,  5,  5,  5,  5,  5,  0,-10,
										-10,  0,  0,  0,  0,  0,  0,-10,
										-20,-10,-10, 0, 0,-10,-10,-20 };

	static const std::array<int, 64> BLACK_KING_MIDDLEGAME = { -30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											 0, 0, -28,  -30,  -30, -28, 0, 0 };

	static const std::array<int, 64> WHITE_KING_MIDDLEGAME = { 0, 0, -28,  -30,  -30, -28, 0, 0,
											 -30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30,
											-30, -30,  -45,  -45,  -45,  -45, -30, -30 };

	static const std::array<int, 64> KING_ENDGAME = { 0, 0, 0,  0,  0, 0, 0, 0,
									   0, 3,  3,  3,  3,  3, 3, 0,
									   5,10,10,10,10,10,10, 5,
									   10,20,20,30,30,20,20, 10,
									   10,20,20,30,30,20,20, 10,
									   5,10,10,10,10,10,10, 5,
									   0, 3,  3,  3,  3,  3, 3, 0,
									   0, 0, 0,  0,  0, 0, 0, 0 };
}

#endif // !CONSTANTS_H_

