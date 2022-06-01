#include "Figure.h"
#include "Rays.h"

#include <cassert>

namespace
{
    std::array<U64, 64> GenerateKnightMoves()
    {
        using namespace bitboards;

        std::array<U64, 64> knightMoves{};
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
    std::array<U64, 64> GenerateKingMoves()
    {
        using namespace bitboards;

        std::array<U64, 64> kingRays;
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

std::list<std::unique_ptr<Figure>> Figure::Create(EFigure figureName, EColor color, std::initializer_list<ESquare> init_squares)
{
    std::list<std::unique_ptr<Figure>> figures;

    for (auto square = init_squares.begin(); square != init_squares.end(); ++square)
    {
        switch (figureName)
        {
        case EFigure::PAWN:
            figures.push_back(std::make_unique<Pawn>(color, *square));
            break;
        case EFigure::KNIGHT:
            figures.push_back(std::make_unique<Knight>(color, *square));
            break;
        case EFigure::BISHOP:
            figures.push_back(std::make_unique<Bishop>(color, *square));
            break;
        case EFigure::ROOK:
            figures.push_back(std::make_unique<Rook>(color, *square));
            break;
        case EFigure::QUEEN:
            figures.push_back(std::make_unique<Queen>(color, *square));
            break;
        case EFigure::KING:
            figures.push_back(std::make_unique<King>(color, *square));
            break;
        }
    }

    return figures;
}

std::unique_ptr<Figure> Figure::Create(EFigure figureName, EColor color, ESquare square)
{
    switch (figureName)
    {
    case EFigure::PAWN: return std::make_unique<Pawn>(color, square);
    case EFigure::KNIGHT: return std::make_unique<Knight>(color, square);
    case EFigure::BISHOP: return std::make_unique<Bishop>(color, square);
    case EFigure::ROOK: return std::make_unique<Rook>(color, square);
    case EFigure::QUEEN: return std::make_unique<Queen>(color, square);
    case EFigure::KING: return std::make_unique<King>(color, square);
    default:
        assert(false);
    }
}

U64 Figure::GetSilentMoves(const U64& blockers, const U64& opposite) const
{
    return GetMoves(blockers, opposite) & ~opposite & ~blockers;
}

U64 Figure::GetCaptureMoves(const U64& blockers, const U64& opposite) const
{
    return GetMoves(blockers, opposite) & opposite;
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

U64 Pawn::GetMoves(const U64& blockers, const U64& opposite) const
{
    U64 moves = 0;
    bool isOnLastLine = (color == EColor::WHITE) ? square >= ESquare::A8 : square <= ESquare::H1;
    if (isOnLastLine) {
        return 0;
    }

    Properties props = (color == EColor::WHITE) ? whitePawn : blackPawn;

    bool isBlockerForward = (opposite | blockers) & TO_BITBOARD((int)square + props.forward);
    if (!isBlockerForward)
    {
        //silent moves
        assert((int)square + props.forward >= 0);
        moves |= TO_BITBOARD((int)square + props.forward);

        bool isTwoSquareMovePossible = !((opposite | blockers) & TO_BITBOARD((int)square + 2 * props.forward)) &&
            (TO_BITBOARD(int(square)) & props.startpos);
        if (isTwoSquareMovePossible) {
            moves |= TO_BITBOARD((int)square + 2 * props.forward);
        }
    }

    //capture moves
    moves |= ((TO_BITBOARD((int)square + props.leftHook) & props.not_rsh_file) |
        (TO_BITBOARD((int)square + props.rightHook)) & props.not_lsh_file) & opposite;

    return moves;
}

U64 Knight::GetMoves(const U64& blockers, const U64& opposite) const
{
    static const std::array<U64, 64> moves = GenerateKnightMoves();
    return moves[(int)square];
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
    const Rook r(color, square);
    const Bishop b(color, square);
    return r.GetMoves(blockers, opposite) | b.GetMoves(blockers, opposite);
}

U64 King::GetMoves(const U64& blockers, const U64& opposite) const
{
    static const std::array<U64, 64> moves = GenerateKingMoves();
    return moves[(int)square];
}

