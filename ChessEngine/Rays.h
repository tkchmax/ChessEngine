#pragma once
#ifndef RAYS_H_
#define RAYS_H_

#include <array>
#include "Constants.h"
#include "EDirection.h"
#include "ESquare.h"

class Rays
{
public:
    using RaysArray = std::array<std::array<U64, 64>, 8>;
    using KnightMoveRays = std::array<U64, 64>;
    using KingMoveRays = std::array<U64, 64>;

    static const Rays& Get();

    const RaysArray& GetRays() const { return rays; }
    const KnightMoveRays& GetKnightMoves() const { return knightMoves; }
    const KnightMoveRays& GetKingMoves() const { return kingMoves; }

private:
    Rays();
    Rays(const Rays&) = delete;
    void operator=(const Rays&) = delete;

private:
    RaysArray rays;
    KnightMoveRays knightMoves;
    KingMoveRays kingMoves;

};

#endif // !RAYS_H_

