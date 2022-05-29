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
}
