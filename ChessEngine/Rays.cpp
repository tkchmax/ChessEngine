#include "Rays.h"
#include "Misc.h"

namespace
{
    U64 _aOut(U64 bitboard)
    {
        return (bitboard << 1) & bitboards::NOT_A_FILE;
    }
    U64 _hOut(U64 bitboard)
    {
        return(bitboard >> 1) & bitboards::NOT_H_FILE;
    }

    Rays::KnightMoveRays GenerateKnightMoveRays()
    {
        using namespace bitboards;

        Rays::KnightMoveRays knightMoves{};
        for (int i = 0; i < 64; ++i)
        {
            std::uint64_t ray = 0;
            ray |= TO_BITBOARD(i + 17) | TO_BITBOARD(i + 15) | TO_BITBOARD(i + 10) | TO_BITBOARD(i + 6);
            if (i >= (int)ESquare::H2) {
                ray |= TO_BITBOARD(i - 16 + 1);
            }
            if (i >= (int)ESquare::B3) {
                ray |= TO_BITBOARD(i - 16 - 1);
            }
            if (i >= (int)ESquare::G1) {
                ray |= (1ULL << i - 8 + 2);
            }
            if (i >= (int)ESquare::C2) {
                ray |= (1ULL << i - 8 - 2);
            }

            if (i % 8 >= 6) {
                ray &= NOT_A_FILE & NOT_B_FILE;
            }
            if (i % 8 <= 1) {
                ray &= NOT_H_FILE & NOT_G_FILE;
            }
            if (i >= (int)ESquare::A7 && i <= (int)ESquare::H8) {
                ray &= NOT_1_RANK & NOT_2_RANK;
            }

            knightMoves[i] = ray;
        }
        return knightMoves;
    }

    Rays::KingMoveRays GenerateKingMoveRays()
    {
        using namespace bitboards;

        Rays::KingMoveRays kingRays;
        for (int i = 0; i < 64; ++i)
        {
            std::uint64_t ray = 0;
            ray |= TO_BITBOARD(i + 8) + TO_BITBOARD(i + 1) | TO_BITBOARD(i + 7) | TO_BITBOARD(i + 9);

            if (i >= (int)ESquare::B1) {
                ray |= TO_BITBOARD(i - 1);
            }
            if (i >= (int)ESquare::H1) {
                ray |= TO_BITBOARD(i - 7);
            }
            if (i >= (int)ESquare::A2) {
                ray |= TO_BITBOARD(i - 8);
            }
            if (i >= (int)ESquare::B2)
                ray |= TO_BITBOARD(i - 9);

            if (i % 8 == 0) {
                ray &= NOT_H_FILE;
            }
            if (i % 8 == 7) {
                ray &= NOT_A_FILE;
            }
            if (i >= (int)ESquare::A8 && i <= (int)ESquare::H8) {
                ray &= NOT_1_RANK;
            }

            kingRays[i] = ray;
        }
        return kingRays;
    }
}

const Rays& Rays::Get()
{
    static Rays instance;
    return instance;
}

Rays::Rays()
{
    std::uint64_t north = 0x0101010101010100;
    std::uint64_t west = 9151314442816847872;
    std::uint64_t south = 0x0080808080808080;
    std::uint64_t east = 254;
    std::uint64_t north_east = 0x8040201008040200;
    std::uint64_t north_west = 72624976668147712;
    std::uint64_t south_west = 18049651735527937;
    std::uint64_t south_east = 567382630219904;

    //North
    for (int i = 0; i < 64; ++i, north <<= 1)
    {
        rays[EDirection::NORTH][i] = north;
    }

    //West
    for (int x = 0; x < 8; ++x)
    {
        std::uint64_t w = west;
        west = _hOut(west);
        for (int y = 63; y >= 0; y -= 8, w >>= 8)
        {
            rays[EDirection::WEST][y - x] = w;
        }
    }

    //South
    for (int i = 63; i >= 0; --i, south >>= 1)
    {
        rays[EDirection::SOUTH][i] = south;
    }

    //East
    for (int x = 0; x < 8; ++x)
    {
        std::uint64_t e = east;
        east = _aOut(east);
        for (int y = 0; y < 64; y += 8, e <<= 8)
        {
            rays[EDirection::EAST][x + y] = e;
        }
    }

    //North-East
    for (int y = 0; y < 8; ++y)
    {
        std::uint64_t ne = north_east;
        north_east = _aOut(north_east);
        for (int x = 0; x < 64; x += 8, ne <<= 8)
        {
            rays[EDirection::NORTH_EAST][x + y] = ne;
        }
    }

    //North-West
    for (int x = 7; x >= 0; --x)
    {
        std::uint64_t nw = north_west;
        north_west = _hOut(north_west);
        for (int y = 0; y < 64; y += 8, nw <<= 8)
        {
            rays[EDirection::NORTH_WEST][y + x] = nw;
        }
    }

    //South-West
    for (int x = 0; x < 8; x++)
    {
        std::uint64_t sw = south_west;
        south_west = _hOut(south_west);
        for (int y = 63; y >= 0; y -= 8, sw >>= 8)
        {
            rays[EDirection::SOUTH_WEST][y - x] = sw;
        }
    }

    //South-East
    for (int x = 7; x >= 0; --x)
    {
        std::uint64_t se = south_east;
        south_east = _aOut(south_east);
        for (int y = 63; y >= 8; y -= 8, se >>= 8)
        {
            rays[EDirection::SOUTH_EAST][y - x] = se;
        }
    }

    knightMoves = GenerateKnightMoveRays();
    kingMoves = GenerateKingMoveRays();
}
