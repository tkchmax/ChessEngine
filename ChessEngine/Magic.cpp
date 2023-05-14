#include "Magic.h"
#include "Rays.h"
#include "Misc.h"
#include <stdexcept>
#include <fstream>

namespace {
    enum {
        Rook,
        Bishop
    };

    U64 GetBishopAttackMask(ESquare square)
    {
        using namespace bitboards;
        U64 cutBits = RANK_1 | RANK_8 | A_FILE | H_FILE;
        return Rays::Get().getBishopRays(square, 0) & ~cutBits;
    }

    U64 GetRookAttackMask(ESquare square)
    {
        using namespace bitboards;
        U64 cutBits = RANK_1 | RANK_8 | A_FILE | H_FILE;
        if (TO_BITBOARD(square) & RANK_1) {
            cutBits &= ~RANK_1;
            cutBits |= TO_BITBOARD(A1) | TO_BITBOARD(H1);
        }
        if (TO_BITBOARD(square) & RANK_8) {
            cutBits &= ~RANK_8;
            cutBits |= TO_BITBOARD(A8) | TO_BITBOARD(H8);
        }
        if (TO_BITBOARD(square) & A_FILE) {
            cutBits &= ~A_FILE;
            cutBits |= TO_BITBOARD(A1) | TO_BITBOARD(A8);
        }
        if (TO_BITBOARD(square) & H_FILE) {
            cutBits &= ~H_FILE;
            cutBits |= TO_BITBOARD(H1) | TO_BITBOARD(H8);
        }

        return Rays::Get().getRookRays(square, 0) & ~cutBits;
    }
}

Magic& Magic::Get()
{
    static Magic instance;
    return instance;
}

Magic::Magic()
{
#ifdef READ_MAGIC
    std::ifstream fin(magic::read_file_name);
    if (!fin.is_open()) {
        throw std::exception("Unable to open file with magic");
    }
    int i = 0;
    std::string line;
    for (int i = 0; i < 64; ++i) {
        getline(fin, line);
        rookMagic[i] = std::stoull(line, nullptr, 10);
    }
    for (int i = 0; i < 64; ++i) {
        getline(fin, line);
        bishopMagic[i] = std::stoull(line, nullptr, 10);
    }
    fin.close();
#else
    //find magic numbers
    for (int square = 0; square < 64; ++square) {
        rookMagic[square] = findMagic(static_cast<ESquare>(square), magic::rook_relevant_bits[square], true);
        bishopMagic[square] = findMagic(static_cast<ESquare>(square), magic::bishop_relevant_bits[square], false);
    }
#endif

    //calculate masks
    for (int square = 0; square < 64; ++square) {
        rookMasks[square] = GetRookAttackMask(static_cast<ESquare>(square));
        bishopMasks[square] = GetBishopAttackMask(static_cast<ESquare>(square));
    }
    
    //write magic to file
    std::ofstream fout;
    fout.open(magic::read_file_name, std::fstream::app);
    if (!fout.is_open()) {
        throw std::exception("Unable to open file to write magic");
    }

    for (int i = 0; i < 64; i++) {
        fout << rookMagic[i] << std::endl;
    }
    for (int i = 0; i < 64; i++) {
        fout << bishopMagic[i] << std::endl;
    }
    fout.close();

    //initialize hash-tables
    initTable(Rook);
    initTable(Bishop);

}

U64 Magic::getRelevantOcc(int iOcc, U64 attackMask) const
{
    U64 occ = 0;
    int nBits = misc::countBits(attackMask);
    for (int i = 0; i < nBits; ++i) {
        int square = misc::lsb(attackMask);
        if (iOcc & TO_BITBOARD(i)) {
            occ |= TO_BITBOARD(square);
        }
        attackMask &= attackMask - 1;
    }
    return occ;
}

U64 Magic::findMagic(ESquare square, int relevantBits, bool isRook) const {
    //RelevantBits - number of bits affect the squares the rook or bishop can move
    constexpr unsigned int MAX_OCC_COMBINATIONS = 1ULL << 12;
    std::array<U64, MAX_OCC_COMBINATIONS> occ;

    //2^relevantBits
    int nOcc = 1ULL << relevantBits;
    U64 attackMask = isRook ? GetRookAttackMask(square) : GetBishopAttackMask(square);

    //iterate over each combination of occupation(blockers on the way),
    //and generate legal rays for the figure.
    std::array<U64, MAX_OCC_COMBINATIONS> legalAttacks;
    for (int i = 0; i < nOcc; ++i) {
        occ[i] = getRelevantOcc(i, attackMask);
        legalAttacks[i] = isRook ?
            Rays::Get().getRookRays(square, occ[i]) :
            Rays::Get().getBishopRays(square, occ[i]);
    }

    //Try to find magic number
    for (int iTry = 0; iTry < 10000000; ++iTry) {
        U64 magicNum = misc::rand64() & misc::rand64() & misc::rand64();

        if (misc::countBits((attackMask * magicNum) & 0xFF00000000000000) < 6) continue;
        
        std::array<U64, MAX_OCC_COMBINATIONS> usedAttacks = {};
        bool fail = false;
        for (int i = 0; !fail && i < nOcc; ++i) {
            int hash = (int)((occ[i] * magicNum) >> (64 - relevantBits));

            if (usedAttacks[hash] == 0) {
                usedAttacks[hash] = legalAttacks[i];
            }
            else if (usedAttacks[hash] != legalAttacks[i]) {
                fail = true;
            }
        }
        
        if (!fail) {
            return magicNum;
        }

    }

    throw std::logic_error("Magic Number Not Found");
}

void Magic::initTable(bool isRook)
{
    for (int square = 0; square < 64; ++square) {
        U64 attackMask = isRook ? rookMasks[square] : bishopMasks[square];
        int relevantBits = isRook ? magic::rook_relevant_bits[square] : magic::bishop_relevant_bits[square];
        int nOcc = 1ULL << relevantBits;
        for (int i = 0; i < nOcc; ++i) {
            U64 occ = getRelevantOcc(i, attackMask);
            if (isRook) {
                int magicIdx = (occ * rookMagic[square]) >> (64 - relevantBits);
                rookTable[square][magicIdx] = Rays::Get().getRookRays(static_cast<ESquare>(square), occ);
            }
            else {
                int magicIdx = (occ * bishopMagic[square]) >> (64 - relevantBits);
                bishopTable[square][magicIdx] = Rays::Get().getBishopRays(static_cast<ESquare>(square), occ);
            }
        }
    }
}
