#include "Figure.h"
#include "Rays.h"

#include <cassert>

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
        //moves &= ~rays[{NORTH_EAST, blockerId}];
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
