#include "Figure.h"
#include "Rays.h"

#include <cassert>

void Figure::move(ESquare newSquare)
{
    squaresCache.push(square);
    square = newSquare;
}

void Figure::moveBack()
{
    assert(squaresCache.size() > 0);
    square = squaresCache.top();
    squaresCache.pop();
}

U64 Pawn::GetMoves(const U64& blockers, const U64& opposite) const
{
    U64 moves = 0;
    bool isOnLastLine = (color == EColor::WHITE) ? square >= ESquare::A8 : square <= ESquare::H1;
    if (isOnLastLine)
    {
        return 0;
    }

    const auto& rays = Rays::Get().GetRays();
    Properties props = (color == EColor::WHITE) ? whitePawn : blackPawn;

    bool isBlockerForward = (opposite | blockers) & TO_BITBOARD((int)square + props.forward);
    if (!isBlockerForward)
    {
        //silent moves
        unsigned int coef = (TO_BITBOARD((int)square) & props.startpos) ? 2 : 1;
        assert((int)square + coef * props.forward >= 0);
        moves |= rays[props.dir][square] & ~rays[props.dir][(int)square + coef * props.forward];
    }

    //capture moves
    moves |= (TO_BITBOARD((int)square + props.leftHook) | TO_BITBOARD((int)square + props.rightHook)) & opposite;

    return moves;
}

U64 Knight::GetMoves(const U64& blockers, const U64& opposite) const
{
    return Rays::Get().GetKnightMoves()[(int)square];
}

U64 Bishop::GetMoves(const U64& blockers, const U64& opposite) const
{
    const auto& rays = Rays::Get().GetRays();

    U64 moves = 0;
    U64 all_pieces = blockers | opposite;

    //North-East
    U64 rayNorthEast = rays[EDirection::NORTH_EAST][square];
    moves |= rayNorthEast;
    if (rayNorthEast & all_pieces)
    {
        int blockerId = misc::BitScanForward(rayNorthEast & all_pieces);
        moves &= ~rays[EDirection::NORTH_EAST][blockerId];
    }

    //North-West
    U64 rayNorthWest = rays[EDirection::NORTH_WEST][square];
    moves |= rayNorthWest;
    if (rayNorthWest & all_pieces)
    {
        int blockerId = misc::BitScanForward(rayNorthWest & all_pieces);
        moves &= ~rays[EDirection::NORTH_WEST][blockerId];
    }

    //South-East
    U64 raySouthEast = rays[SOUTH_EAST][square];
    moves |= raySouthEast;
    if (raySouthEast & all_pieces)
    {
        int blockerId = misc::BitScanReverse(raySouthEast & all_pieces);
        moves &= ~rays[EDirection::SOUTH_EAST][blockerId];
    }

    //South-West
    U64 raySouthWest = rays[SOUTH_WEST][square];
    moves |= raySouthWest;
    if (raySouthWest & all_pieces)
    {
        int blockerId = misc::BitScanReverse(raySouthWest & all_pieces);
        moves &= ~rays[EDirection::SOUTH_WEST][blockerId];
    }

    return moves;
}

U64 Rook::GetMoves(const U64& blockers, const U64& opposite) const
{
    const auto& rays = Rays::Get().GetRays();

    std::uint64_t moves = 0;
    std::uint64_t all_pieces = blockers | opposite;

    //North
    U64 rayNorth = rays[EDirection::NORTH][square];
    moves |= rayNorth;
    if (rayNorth & all_pieces)
    {
        int blockerId = misc::BitScanForward(rayNorth & all_pieces);
        moves &= ~rays[EDirection::NORTH][blockerId];
    }

    //West
    U64 rayWest = rays[EDirection::WEST][square];
    moves |= rayWest;
    if (rayWest & all_pieces)
    {
        int blockerId = misc::BitScanReverse(rayWest & all_pieces);
        moves &= ~rays[EDirection::WEST][blockerId];
    }

    //South
    U64 raySouth = rays[EDirection::SOUTH][square];
    moves |= raySouth;
    if (raySouth & all_pieces)
    {
        int blockerId = misc::BitScanReverse(raySouth & all_pieces);
        moves &= ~rays[EDirection::SOUTH][blockerId];
    }

    //East
    U64 rayEast = rays[EDirection::EAST][square];
    moves |= rayEast;
    if (rayEast & all_pieces)
    {
        int blockerId = misc::BitScanForward(rayEast & all_pieces);
        moves &= ~rays[EDirection::EAST][blockerId];
    }

    return moves;
}

U64 Queen::GetMoves(const U64& blockers, const U64& opposite) const
{
    static const Rook r(color, square);
    static const Bishop b(color, square);
    return r.GetMoves(blockers, opposite) | b.GetMoves(blockers, opposite);
}
