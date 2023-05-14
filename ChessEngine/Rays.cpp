#include "Rays.h"
#include "Misc.h"

namespace {
    U64 _aOut(U64 bitboard)
    {
        return (bitboard << 1) & ~bitboards::A_FILE;
    }
    U64 _hOut(U64 bitboard)
    {
        return(bitboard >> 1) & ~bitboards::H_FILE;
    }

    U64 GetKingRays(ESquare sq)
    {
        using namespace bitboards;
        std::uint64_t ray = 0;
        ray |= TO_BITBOARD(sq + 8) + TO_BITBOARD(sq + 1) | TO_BITBOARD(sq + 7) | TO_BITBOARD(sq + 9);

        if (sq >= (int)ESquare::B1) {
            ray |= TO_BITBOARD(sq - 1);
        }
        if (sq >= (int)ESquare::H1) {
            ray |= TO_BITBOARD(sq - 7);
        }
        if (sq >= (int)ESquare::A2) {
            ray |= TO_BITBOARD(sq - 8);
        }
        if (sq >= (int)ESquare::B2)
            ray |= TO_BITBOARD(sq - 9);

        if (sq % 8 == 0) {
            ray &= ~H_FILE;
        }
        if (sq % 8 == 7) {
            ray &= ~A_FILE;
        }
        if (sq >= (int)ESquare::A8 && sq <= (int)ESquare::H8) {
            ray &= ~RANK_1;
        }

        return ray;
    }

    U64 GetKnightRays(ESquare sq)
    {
        using namespace bitboards;
        std::uint64_t ray = 0;
        ray |= TO_BITBOARD(sq + 17) | TO_BITBOARD(sq + 15) | TO_BITBOARD(sq + 10) | TO_BITBOARD(sq + 6);

        if (sq >= (int)ESquare::H2) {
            ray |= TO_BITBOARD(sq - 16 + 1);
        }
        if (sq >= (int)ESquare::B3) {
            ray |= TO_BITBOARD(sq - 16 - 1);
        }
        if (sq >= (int)ESquare::G1) {
            ray |= (1ULL << sq - 8 + 2);
        }
        if (sq >= (int)ESquare::C2) {
            ray |= (1ULL << sq - 8 - 2);
        }

        if (sq % 8 >= 6) {
            ray &= ~A_FILE & ~B_FILE;
        }
        if (sq % 8 <= 1) {
            ray &= ~H_FILE & ~G_FILE;
        }
        if (sq >= (int)ESquare::A7 && sq <= (int)ESquare::H8) {
            ray &= ~RANK_1 & ~RANK_2;
        }

        return ray;
    }

    U64 GetPawnRays(EColor color, ESquare sq) {
        return color == WHITE ?
            (TO_BITBOARD(sq + 7) & ~bitboards::H_FILE) | TO_BITBOARD(sq + 8) | (TO_BITBOARD(sq + 9) & ~bitboards::A_FILE) :
            (TO_BITBOARD(sq - 7) & ~bitboards::A_FILE) | TO_BITBOARD(sq - 8) | (TO_BITBOARD(sq - 9) & ~bitboards::H_FILE);
    }
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
        rays[EDir::NORTH][i] = north;
    }

    //West
    for (int x = 0; x < 8; ++x)
    {
        std::uint64_t w = west;
        west = _hOut(west);
        for (int y = 63; y >= 0; y -= 8, w >>= 8)
        {
            rays[EDir::WEST][y - x] = w;
        }
    }

    //South
    for (int i = 63; i >= 0; --i, south >>= 1)
    {
        rays[EDir::SOUTH][i] = south;
    }

    //East
    for (int x = 0; x < 8; ++x)
    {
        std::uint64_t e = east;
        east = _aOut(east);
        for (int y = 0; y < 64; y += 8, e <<= 8)
        {
            rays[EDir::EAST][x + y] = e;
        }
    }

    //North-East
    for (int y = 0; y < 8; ++y)
    {
        std::uint64_t ne = north_east;
        north_east = _aOut(north_east);
        for (int x = 0; x < 64; x += 8, ne <<= 8)
        {
            rays[EDir::NORTH_EAST][x + y] = ne;
        }
    }

    //North-West
    for (int x = 7; x >= 0; --x)
    {
        std::uint64_t nw = north_west;
        north_west = _hOut(north_west);
        for (int y = 0; y < 64; y += 8, nw <<= 8)
        {
            rays[EDir::NORTH_WEST][y + x] = nw;
        }
    }

    //South-West
    for (int x = 0; x < 8; x++)
    {
        std::uint64_t sw = south_west;
        south_west = _hOut(south_west);
        for (int y = 63; y >= 0; y -= 8, sw >>= 8)
        {
            rays[EDir::SOUTH_WEST][y - x] = sw;
        }
    }

    //South-East
    for (int x = 7; x >= 0; --x)
    {
        std::uint64_t se = south_east;
        south_east = _aOut(south_east);
        for (int y = 63; y >= 8; y -= 8, se >>= 8)
        {
            rays[EDir::SOUTH_EAST][y - x] = se;
        }
    }

    //Generate pseudo attacks for each figure
    for (int sq = 0; sq < SQ_NB; ++sq) {
        figurePseudoAttacks[ROOK][sq] = getRookRays(ESquare(sq), 0);
        figurePseudoAttacks[BISHOP][sq] = getBishopRays(ESquare(sq), 0);
        figurePseudoAttacks[QUEEN][sq] = getQueenRays(ESquare(sq), 0);

        figurePseudoAttacks[KNIGHT][sq] = GetKnightRays(ESquare(sq));
        figurePseudoAttacks[KING][sq] = GetKingRays(ESquare(sq));
        pawnPseudoAttacks[WHITE][sq] = GetPawnRays(WHITE, ESquare(sq));
        pawnPseudoAttacks[BLACK][sq] = GetPawnRays(BLACK, ESquare(sq));
    }
}

const Rays& Rays::Get()
{
    static Rays instance;
    return instance;
}

U64 Rays::getRookRays(ESquare square, const U64& occ) const
{
    std::uint64_t rookRays = 0;

    //North
    U64 rayNorth = rays[NORTH][square];
    rookRays |= rayNorth;
    if (rayNorth & occ)
    {
        int blockerId = misc::lsb(rayNorth & occ);
        rookRays &= ~rays[NORTH][blockerId];
    }

    //West
    U64 rayWest = rays[WEST][square];
    rookRays |= rayWest;
    if (rayWest & occ)
    {
        int blockerId = misc::msb(rayWest & occ);
        rookRays &= ~rays[WEST][blockerId];
    }

    //South
    U64 raySouth = rays[SOUTH][square];
    rookRays |= raySouth;
    if (raySouth & occ)
    {
        int blockerId = misc::msb(raySouth & occ);
        rookRays &= ~rays[SOUTH][blockerId];
    }

    //East
    U64 rayEast = rays[EAST][square];
    rookRays |= rayEast;
    if (rayEast & occ)
    {
        int blockerId = misc::lsb(rayEast & occ);
        rookRays &= ~rays[EAST][blockerId];
    }

    return rookRays;
}

U64 Rays::getBishopRays(ESquare square, const U64& occ) const
{
    U64 bishopRays = 0;

    //North-East
    U64 rayNorthEast = rays[NORTH_EAST][square];
    bishopRays |= rayNorthEast;
    if (rayNorthEast & occ)
    {
        int blockerId = misc::lsb(rayNorthEast & occ);
        bishopRays &= ~rays[NORTH_EAST][blockerId];
    }

    //North-West
    U64 rayNorthWest = rays[NORTH_WEST][square];
    bishopRays |= rayNorthWest;
    if (rayNorthWest & occ)
    {
        int blockerId = misc::lsb(rayNorthWest & occ);
        bishopRays &= ~rays[NORTH_WEST][blockerId];
    }

    //South-East
    U64 raySouthEast = rays[SOUTH_EAST][square];
    bishopRays |= raySouthEast;
    if (raySouthEast & occ)
    {
        int blockerId = misc::msb(raySouthEast & occ);
        bishopRays &= ~rays[SOUTH_EAST][blockerId];
    }

    //South-West
    U64 raySouthWest = rays[SOUTH_WEST][square];
    bishopRays |= raySouthWest;
    if (raySouthWest & occ)
    {
        int blockerId = misc::msb(raySouthWest & occ);
        bishopRays &= ~rays[SOUTH_WEST][blockerId];
    }

    return bishopRays;
}

U64 Rays::getQueenRays(ESquare square, const U64& occ) const
{
    return getRookRays(square, occ) | getBishopRays(square, occ);
}


