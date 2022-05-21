#include "Board.h"
#include <iostream>
Board::Board()
{
    figures[WHITE][PAWN] = std::move(Figure::Create(PAWN, WHITE, { A2,B2,C2,D2,E2,F2,G2,H2 }));
    figures[BLACK][PAWN] = std::move(Figure::Create(PAWN, BLACK, { A7,B7,C7,D7,E7,F7,G7,H7 }));
    figures[WHITE][KNIGHT] = std::move(Figure::Create(KNIGHT, WHITE, { B1, G1 }));
    figures[BLACK][KNIGHT] = std::move(Figure::Create(KNIGHT, BLACK, { B8,G8 }));
    figures[WHITE][BISHOP] = std::move(Figure::Create(BISHOP, WHITE, { C1, F1 }));
    figures[BLACK][BISHOP] = std::move(Figure::Create(BISHOP, BLACK, { C8, F8 }));
    figures[WHITE][ROOK] = std::move(Figure::Create(ROOK, WHITE, { A1, H1}));
    figures[BLACK][ROOK] = std::move(Figure::Create(ROOK, BLACK, { A8,H8 }));
    figures[WHITE][QUEEN] = std::move(Figure::Create(QUEEN, WHITE, { D1 }));
    figures[BLACK][QUEEN] = std::move(Figure::Create(QUEEN, BLACK, { D8 }));
    figures[WHITE][KING] = std::move(Figure::Create(KING, WHITE, { E1 }));
    figures[BLACK][KING] = std::move(Figure::Create(KING, BLACK, { E8 }));

    for (unsigned colorInt = 0; colorInt < 2; ++colorInt) {
        for (unsigned squareInt = 0; squareInt < 64; ++squareInt) {
            figureFromCoord[colorInt][squareInt] = EFigure::NO_FIGURE;
        }
    }

    for (unsigned colorInt = 0; colorInt < 2; ++colorInt)
    {
        for (unsigned figureInt = 0; figureInt < EFigure::COUNT; ++figureInt)
        {
            const std::list<std::unique_ptr<Figure>>& list = figures[colorInt][figureInt];
            for (auto it = list.begin(); it != list.end(); ++it)
            {
                unsigned squareInt = (*it)->GetSquare();
                figureFromCoord[colorInt][squareInt] = figureInt;
            }
        }
    }
    std::cout << "";
}

U64 Board::GetSideBoard(EColor color) const
{
    U64 sideBoard = 0;
    for (int figureInt = 0; figureInt < (int)EFigure::COUNT; ++figureInt)
    {
        const auto& list = figures[color][figureInt];
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            sideBoard |= TO_BITBOARD((*it)->GetSquare());
        }
    }
    return sideBoard;
}

U64 Board::GetAttackRays(EColor color) const
{
    U64 blockers = GetSideBoard(color);
    U64 opposite = GetSideBoard((color == WHITE) ? BLACK : WHITE);

    U64 attackRays = 0;
    for (unsigned figureInt = 0; figureInt < EFigure::COUNT; ++figureInt) {
        const auto& list = figures[color][figureInt];
        for (const auto& figure : list) {
            attackRays |= figure->GetMoves(blockers, opposite) & ~blockers;
        }
    }
    return attackRays;
}

bool Board::IsKingAttacked(EColor color) const
{
    U64 attackRays = GetAttackRays((color == WHITE) ? BLACK : WHITE);
    auto kingIter = figures[color][KING].begin();

    return TO_BITBOARD((*kingIter)->GetSquare()) & attackRays;
}
