#include "Figure.h"
#include "Rays.h"

#include <cassert>

U64 Bishop::GetMoves(const U64& blockers, const U64& opposite) const
{
    const auto& rays = Rays::Get().GetRays();

    U64 moves = 0;
    U64 all_pieces = blockers | opposite;

    //North-East
    auto rayNorthEast = rays[EDirection::NORTH_EAST][square];
    moves |= rayNorthEast;
    if (rayNorthEast & all_pieces)
    {
        int blockerId = misc::BitScanForward(rayNorthEast & all_pieces);
        //moves &= ~rays[{NORTH_EAST, blockerId}];
        moves &= ~rays[EDirection::NORTH_EAST][blockerId];
    }

    //North-West
    auto rayNorthWest = rays[EDirection::NORTH_WEST][square];
    moves |= rayNorthWest;
    if (rayNorthWest & all_pieces)
    {
        int blockerId = misc::BitScanForward(rayNorthWest & all_pieces);
        moves &= ~rays[EDirection::NORTH_WEST][blockerId];
    }

    //South-East
    auto raySouthEast = rays[SOUTH_EAST][square];
    moves |= raySouthEast;
    if (raySouthEast & all_pieces)
    {
        int blockerId = misc::BitScanReverse(raySouthEast & all_pieces);
        moves &= ~rays[EDirection::SOUTH_EAST][blockerId];
    }

    //South-West
    auto raySouthWest = rays[SOUTH_WEST][square];
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
