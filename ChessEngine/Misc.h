#pragma once
#ifndef MISC_H_
#define MISC_H_
#include "Constants.h"
#include <string>
#include <cassert>

#define WRITE_FROM(from,num) (num|from)
#define WRITE_TO(to,num) (num|(to<<6))
#define WRITE_FIGURE(figure,num) (num|(figure<<12))

#define READ_FROM(num) (num&63)
#define READ_TO(num) ((num>>6)&63)
#define READ_FIGURE(num) ((num>>12)&7)
#define READ_MOVE_TYPE(num) ((num >> 15) & 7)

#define TO_BITBOARD(num) (U64)(1ULL << (num))

enum EMoveType;

namespace misc
{
    template<EDir dir>
    constexpr U64 shift_bb(U64 bb) {
        using namespace bitboards;
        return dir == NORTH ? bb << 8 : dir == SOUTH ? bb >> 8
            : dir == NORTH_NORTH ? bb << 16 : dir == SOUTH_SOUTH ? bb >> 16
            : dir == EAST ? (bb & ~H_FILE) << 1 : dir == WEST ? (bb & ~A_FILE) >> 1
            : dir == NORTH_EAST ? (bb & ~H_FILE) << 9
            : dir == NORTH_WEST ? (bb & ~A_FILE) << 7
            : dir == SOUTH_EAST ? (bb & ~H_FILE) >> 7
            : dir == SOUTH_WEST ? (bb & ~A_FILE) >> 9
            : 0;
    }

    template<EColor color>
    constexpr EDir relative_dir(EDir dir) {
        return EDir(
            (color == WHITE && dir % 2 == 0) ? dir :
            (color == BLACK && dir % 2 != 0) ? dir :
            (color == BLACK && dir % 2 == 0) ? dir + 1 :
            (color == WHITE && dir % 2 != 0) ? dir - 1 :
            0);
    }

    template<EDir dir>
    constexpr int shift_value() {
        return dir == NORTH ? 8 :
            dir == SOUTH ? -8 :
            dir == WEST ? -1 :
            dir == EAST ? 1 :
            dir == NORTH_EAST ? 9 :
            dir == SOUTH_EAST ? -7 :
            dir == NORTH_WEST ? 7 :
            dir == SOUTH_WEST ? -9 :
            dir == NORTH_NORTH ? 16 :
            dir == SOUTH_SOUTH ? -16 :
            0;
    }

    void ShowBits(const U64& bitboard);

    inline unsigned long lsb(U64 bb) {
        assert(bb);
        unsigned long index;
        _BitScanForward64(&index, bb);
        return index;
    }

    inline unsigned long msb(U64 bb) {
        assert(bb);
        unsigned long index;
        _BitScanReverse64(&index, bb);
        return index;
    }

    inline ESquare pop_lsb(U64& bb) {
        ESquare s = static_cast<ESquare>(lsb(bb));
        bb &= bb - 1;
        return s;
    }

    U64 rand64();
    int countBits(U64 num);

    std::string ToString(ESquare square);
    std::string ToString(EFigure figure);
    std::string ToString(EFigureType type);
    std::string ToString(EColor color);
    std::string ToString(EMoveType movetype);
    std::string ToString(Move move);
}

#endif //MISC_H_
