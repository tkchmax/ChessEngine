#pragma once
#ifndef RAYS_H_
#define RAYS_H_
#include "Constants.h"

class Rays
{
public:
    using RaysArray = U64[8][SQ_NB];
    using PseudoRays = U64[FIGURE_NB][SQ_NB];
    using PawnRays = U64[COLOR_NB][SQ_NB];
    using SquareMask = U64[SQ_NB];
    using PassedPawnMask = U64[COLOR_NB][SQ_NB];

    static const Rays& Get();
    const RaysArray& getRays() const;
    const PseudoRays& getPseudoAttacks() const;
    const PawnRays& getPawnPseusoAttacks() const;
    const SquareMask& getFileMask() const;
    const PassedPawnMask& getPassedPawnMask() const;
    U64 getRookRays(ESquare square, const U64& occ) const;
    U64 getBishopRays(ESquare square, const U64& occ) const;
    U64 getQueenRays(ESquare square, const U64& occ) const;

private:
    Rays();
    Rays(const Rays&) = delete;
    void operator=(const Rays&) = delete;

private:
    RaysArray rays;
    PseudoRays figurePseudoAttacks;
    PawnRays pawnPseudoAttacks;
    SquareMask fileMask;
    PassedPawnMask passedPawnMask;
};

inline const Rays::RaysArray& Rays::getRays() const { return rays; }

inline const Rays::PseudoRays& Rays::getPseudoAttacks() const { return figurePseudoAttacks; }

inline const Rays::PawnRays& Rays::getPawnPseusoAttacks() const { return pawnPseudoAttacks; }

inline const Rays::SquareMask& Rays::getFileMask() const { return fileMask;  }

inline const Rays::PassedPawnMask& Rays::getPassedPawnMask() const
{
    return passedPawnMask;
}

#endif //RAYS_H_
