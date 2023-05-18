#pragma once
#ifndef MAGIC_H_
#define MAGIC_H_
#include "Rays.h"

class Magic {
public:
    static Magic& Get();
    U64 getRookAttack(ESquare square, U64 occ) const;
    U64 getBishopAttack(ESquare square, U64 occ) const;
private:
    Magic();
    Magic(const Magic&) = delete;
    void operator=(const Magic&) = delete;
    U64 getRelevantOcc(int iOcc, U64 attackMask) const;
    U64 findMagic(ESquare square, int relevantBits, bool isRook) const;
    void initTable(bool isRook);
private:
    std::array<U64, 64> rookMagic, bishopMagic;
    std::array<U64, 64> rookMasks, bishopMasks;
    std::array<std::array<U64, 4096>, 64> rookTable;
    std::array<std::array<U64, 512>, 64> bishopTable;
};

inline U64 Magic::getRookAttack(ESquare square, U64 occ) const {
    occ &= rookMasks[square];
    occ *= rookMagic[square];
    occ >>= (64 - magic::rook_relevant_bits[square]);
    return rookTable[square][occ];
}

inline U64 Magic::getBishopAttack(ESquare square, U64 occ) const {
    occ &= bishopMasks[square];
    occ *= bishopMagic[square];
    occ >>= (64 - magic::bishop_relevant_bits[square]);
    return bishopTable[square][occ];
}

template <EFigureType fig_type>
inline U64 GetAttackBB(ESquare sq, U64 occ = 0) {
    assert(fig_type != PAWN);

    switch (fig_type) {
    case EFigureType::BISHOP: return Magic::Get().getBishopAttack(sq, occ);
    case EFigureType::ROOK:   return Magic::Get().getRookAttack(sq, occ);
    case EFigureType::QUEEN:  return GetAttackBB<BISHOP>(sq, occ) | GetAttackBB<ROOK>(sq, occ);
    default: return Rays::Get().getPseudoAttacks()[fig_type][sq];
    }
}

#endif //MAGIC_H_