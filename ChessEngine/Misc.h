#pragma once

#ifndef MISC_H_
#define MISC_H_

#include <stdint.h>

#define WRITE_FROM(from,num) (num|from)
#define WRITE_TO(to,num) (num|(to<<6))
#define WRITE_FIGURE(figure,num) (num|(figure<<12))
#define WRITE_CAPTURE(capture,num) (num|(capture<<15))
#define WRITE_MOVE_TYPE(mt, num) (num|(mt<<18))
#define WRITE_COLOR(color, num) (num|(color<<22))

#define READ_FROM(num) (num&63)
#define READ_TO(num) ((num>>6)&63)
#define READ_FIGURE(num) ((num>>12)&7)
#define READ_CAPTURE(num) ((num>>15)&7)
#define READ_MOVE_TYPE(num) ((num>>18)&15)
#define READ_COLOR(num) ((num>>22)& 1)

using U64 = uint64_t;
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

#endif // !MISC_H_

