#pragma once
#ifndef CONSTANTS_H_
#define CONSTANTS_H_
#include <stdint.h>
#include <array>

#define INF 10000000
#define MAX_PLY 30

using U64 = uint64_t;
using U32 = uint32_t;
using Move = U32;

enum EColor : int {
    WHITE, BLACK, COLOR_NB
};

constexpr EColor operator~(EColor color) {
    return EColor(color ^ BLACK);
}

enum ESquare : int {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    SQ_NONE,
    SQ_NB = 64
};

enum EFigureType : int {
    NO_FIGURE_TYPE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING,
    ALL_FIGURE_TYPE, FIGURE_TYPE_NB = 8
};

enum EFigure : int {
    NO_FIGURE, W_PAWN = PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
    B_PAWN = PAWN + 8, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
    FIGURE_NB
};

constexpr EFigureType type_of(EFigure f) {
    return EFigureType(f & 7);
}

constexpr EColor color_of(EFigure f) {
    return EColor(f >> 3);
}

constexpr EFigure add_color(EColor color, EFigureType fig_type) {
    return EFigure((color << 3) + fig_type);
}

enum EDir : int {
    NORTH, SOUTH,
    WEST, EAST,
    NORTH_EAST, SOUTH_WEST,
    NORTH_WEST, SOUTH_EAST,
    NORTH_NORTH, SOUTH_SOUTH
};

enum EGenType {
    QUIETS,
    CAPTURES,
    PSEUDO,
    LEGAL
};

enum CastlingRights {
    NO_CASTLING = 0,
    WHITE_00 = 1,
    WHITE_000 = WHITE_00 << 1,
    BLACK_00 = WHITE_00 << 2,
    BLACK_000 = WHITE_00 << 3,

    //WHITE_CASTLING = WHITE_00 | WHITE_000,
    //BLACK_CASTLING = BLACK_00 | BLACK_000,
    CR_NB
};

enum EGamePhase {
    OPENNING,
    END_GAME,
    MIDDLE_GAME,
    GAME_PHASE_NB
};

namespace bitboards
{
    constexpr U64 H_FILE = 0x8080808080808080;
    constexpr U64 A_FILE = 0x0101010101010101;
    constexpr U64 G_FILE = 4629771061636907072;
    constexpr U64 B_FILE = 144680345676153346;
    constexpr U64 RANK_1 = 0xFF;
    constexpr U64 RANK_2 = 0xFF00;
    constexpr U64 RANK_4 = 0xFF000000;
    constexpr U64 RANK_5 = 0xFF00000000;
    constexpr U64 RANK_8 = 0xFF00000000000000;
    constexpr U64 RANK_7 = 0xFF000000000000;
    constexpr U64 pawn_transformation_line[2] = { RANK_8, RANK_1 };


    namespace startpos
    {
        constexpr U64  WHITE_PAWN = 0xFF00;
        constexpr U64  BLACK_PAWN = 0xFF000000000000;
        constexpr U64  WHITE_KNIGHT = 0x42;
        constexpr U64  BLACK_KNIGHT = 0x4200000000000000;
        constexpr U64  WHITE_BISHOP = 0x24;
        constexpr U64  BLACK_BISHOP = 0x2400000000000000;
        constexpr U64  WHITE_ROOK = 0x81;
        constexpr U64  BLACK_ROOK = 0x8100000000000000;
        constexpr U64  WHITE_QUEEN = 0x8;
        constexpr U64  BLACK_QUEEN = 0x800000000000000;
        constexpr U64  WHITE_KING = 0x10;
        constexpr U64  BLACK_KING = 0x1000000000000000;
    }
}

namespace castling {

    constexpr U64 W_CASTLING00_BLOCKERS_MASK = 0x60;
    constexpr U64 B_CASTLING00_BLOCKERS_MASK = 0x6000000000000000;
    constexpr U64 W_CASTLING000_BLOCKERS_MASK = 0xE;
    constexpr U64 B_CASTLING000_BLOCKERS_MASK = 0xE00000000000000;

    constexpr U64 pathBlockers[CastlingRights::CR_NB] = { 0,W_CASTLING00_BLOCKERS_MASK, W_CASTLING000_BLOCKERS_MASK, 0,
                                                      B_CASTLING00_BLOCKERS_MASK, 0,0,0, B_CASTLING000_BLOCKERS_MASK };

    constexpr U64 pathBlockers00[COLOR_NB] = { W_CASTLING00_BLOCKERS_MASK, B_CASTLING00_BLOCKERS_MASK };

    constexpr U64 pathBlockers000[COLOR_NB] = { W_CASTLING000_BLOCKERS_MASK, B_CASTLING000_BLOCKERS_MASK };

    constexpr ESquare rookSquare[CastlingRights::CR_NB] = { SQ_NONE, H1, A1, SQ_NONE,
                                                       H8, SQ_NONE,SQ_NONE,SQ_NONE, A8 };

    constexpr CastlingRights byColor[2][2] = { {WHITE_00, WHITE_000}, {BLACK_00, BLACK_000} };

    //[color][fromTo]
    constexpr ESquare rookFromTo00[2][2] = { {H1, F1}, {H8, F8} };
    constexpr ESquare rookFromTo000[2][2] = { {A1, D1}, {A8, D8} };
    constexpr ESquare kingTo00[2] = { G1, G8 };
    constexpr ESquare kingTo000[2] = { C1, C8 };
}

namespace cmove_list {
    constexpr int capacity = 256;
}

namespace magic {
    constexpr const char* read_file_name = "magic.txt";

    constexpr int bishop_relevant_bits[64] = {
                        6,5,5,5,5,5,5,6,
                        5,5,5,5,5,5,5,5,
                        5,5,7,7,7,7,5,5,
                        5,5,7,9,9,7,5,5,
                        5,5,7,9,9,7,5,5,
                        5,5,7,7,7,7,5,5,
                        5,5,5,5,5,5,5,5,
                        6,5,5,5,5,5,5,6 };

    constexpr int rook_relevant_bits[64] = {
                        12,11,11,11,11,11,11,12,
                        11,10,10,10,10,10,10,11,
                        11,10,10,10,10,10,10,11,
                        11,10,10,10,10,10,10,11,
                        11,10,10,10,10,10,10,11,
                        11,10,10,10,10,10,10,11,
                        11,10,10,10,10,10,10,11,
                        12,11,11,11,11,11,11,12 };
}

namespace figure
{
    constexpr int PAWN_COST = 100;
    constexpr int KNIGHT_COST = 300;
    constexpr int BISHOP_COST = 301;
    constexpr int ROOK_COST = 500;
    constexpr int QUEEN_COST = 1000;
    constexpr int KING_COST = 10000;
    constexpr int costs[7] = { 0, PAWN_COST,KNIGHT_COST,BISHOP_COST,ROOK_COST,QUEEN_COST,KING_COST };

    static const std::array<int, 64> WHITE_PAWN_PR = { 0, 0, 0, 0, 0, 0, 0, 0,
                                                    4, 4, 4, 0, 0, 4, 4, 4,
                                                    6, 8, 2, 10,10,2, 8, 6,
                                                    6, 8, 12,16,16,12,8, 6,
                                                    6, 12,16,24,24,16,12,8,
                                                    12, 16,24,32,32,24,16,12,
                                                    15, 20,30,35,35,30,20,15,
                                                    0, 0, 0, 0, 0, 0, 0, 0 };

    static const std::array<int, 64> BLACK_PAWN_PR = { 0, 0, 0, 0, 0, 0, 0, 0,
                                                    15, 20,30,35,35,30,20,15,
                                                    12, 16,24, 32,32,24,16,12,
                                                    6,  12,16, 24,24,16,12, 8,
                                                    6,	8, 12, 16,16,12, 8, 6,
                                                    6,  8, 2,  10,10, 2, 8, 6,
                                                    4,  4, 4,  0,  0, 4, 4, 4,
                                                    0,  0, 0,  0,  0, 0, 0, 0 };

    static const std::array<int, 64> KNIGHT_PR = { 0, 2, 8, 10, 10, 8, 2, 0,
                                                4 ,8, 16, 20, 20, 16, 8, 4,
                                                8, 16, 25, 28, 28, 25, 16,8,
                                                10, 20,28, 32, 32, 28, 20,10,
                                                10, 20,28, 32, 32, 28, 20,10,
                                                8, 16, 25, 28, 28, 25, 16, 8,
                                                4, 8, 16, 20, 20, 16,  8,  4,
                                                0, 2, 8,  10, 10  ,8  ,2  ,0 };

    static const std::array<int, 64> BISHOP_PR = { 0,	0,	0,	0,	0,	0,	0,	0,
                                                5,  23,  15,  15,  15,  15,  23,5,
                                                5,  18,  22, 22, 22,  22,  18,5,
                                                5,  18,  22, 22, 22,  22,   18,5,
                                                5,  18, 22, 22, 22,  22,   18,5,
                                                5, 18,22, 22, 22,   22, 18,	5,
                                                5,  23,  15,  15,  15,  15,  23,5,
                                                0,	0,	0,	0,	0,	0,	0,	0 };

    static const std::array<int, 64> ROOK_PR = { 0, 0, 0, 5, 5, 5, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 5, 5, 5, 0, 0 };

    static const std::array<int, 64> QUEEN_PR = { -20,-10,-10, 0, 0,-10,-10,-20,
                                            -10,  0,  0,  0,  0,  0,  0,-10,
                                            -10,  0,  5,  5,  5,  5,  0,-10,
                                             -5,  0,  5,  5,  5,  5,  0, -5,
                                             -5,  0,  5,  5,  5,  5,  0, -5,
                                            -10,  5,  5,  5,  5,  5,  0,-10,
                                            -10,  0,  0,  0,  0,  0,  0,-10,
                                            -20,-10,-10, 0, 0,-10,-10,-20 };

    static const std::array<int, 64> BLACK_KING_MIDDLEGAME_PR = { -30, -30,  -45,  -45,  -45,  -45, -30, -30,
                                            -30, -30,  -45,  -45,  -45,  -45, -30, -30,
                                            -30, -30,  -45,  -45,  -45,  -45, -30, -30,
                                            -30, -30,  -45,  -45,  -45,  -45, -30, -30,
                                            -30, -30,  -45,  -45,  -45,  -45, -30, -30,
                                            -30, -30,  -45,  -45,  -45,  -45, -30, -30,
                                            -30, -30,  -45,  -45,  -45,  -45, -30, -30,
                                             0, 0, -28,  -30,  -30, -28, 0, 0 };

    static const std::array<int, 64> WHITE_KING_MIDDLEGAME_PR = { 0, 0, -28,  -30,  -30, -28, 0, 0,
                                             -30, -30,  -45,  -45,  -45,  -45, -30, -30,
                                            -30, -30,  -45,  -45,  -45,  -45, -30, -30,
                                            -30, -30,  -45,  -45,  -45,  -45, -30, -30,
                                            -30, -30,  -45,  -45,  -45,  -45, -30, -30,
                                            -30, -30,  -45,  -45,  -45,  -45, -30, -30,
                                            -30, -30,  -45,  -45,  -45,  -45, -30, -30,
                                            -30, -30,  -45,  -45,  -45,  -45, -30, -30 };

    static const std::array<int, 64> KING_ENDGAME_PR = { 0, 0, 0,  0,  0, 0, 0, 0,
                                       0, 3,  3,  3,  3,  3, 3, 0,
                                       5,10,10,10,10,10,10, 5,
                                       10,20,20,30,30,20,20, 10,
                                       10,20,20,30,30,20,20, 10,
                                       5,10,10,10,10,10,10, 5,
                                       0, 3,  3,  3,  3,  3, 3, 0,
                                       0, 0, 0,  0,  0, 0, 0, 0 };


}

namespace evaluate {

    constexpr int opening_bounder_score = 6192;
    constexpr int endgame_bounder_score = 518;

    // material score [game phase][piece]
    constexpr int material_score[2][FIGURE_NB]
    {
        // opening material score
        0, 82, 337, 365, 477, 1025, 12000, 0, 0, -82, -337, -365, -477, -1025, -12000,

        // endgame material score
        0, 94, 281, 297, 512,  936, 12000, 0 , 0, -94, -281, -297, -512,  -936, -12000
    };
}

#endif //CONSTANTS_H_
