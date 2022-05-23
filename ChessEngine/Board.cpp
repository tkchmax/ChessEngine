#include "Board.h"
#include <cassert>
#include <iostream>
#include <bitset>

Board::Board()
{
    figures[WHITE][PAWN] = std::move(Figure::Create(PAWN, WHITE, { A2,B2,C2,D2,E2,F2,G2,H2 }));
    figures[BLACK][PAWN] = std::move(Figure::Create(PAWN, BLACK, { A7,B7,C7,D7,E7,F7,G7,H7 }));
    figures[WHITE][KNIGHT] = std::move(Figure::Create(KNIGHT, WHITE, { B1, G1 }));
    figures[BLACK][KNIGHT] = std::move(Figure::Create(KNIGHT, BLACK, { B8,G8 }));
    figures[WHITE][BISHOP] = std::move(Figure::Create(BISHOP, WHITE, { C1, F1 }));
    figures[BLACK][BISHOP] = std::move(Figure::Create(BISHOP, BLACK, { C8, F8 }));
    figures[WHITE][ROOK] = std::move(Figure::Create(ROOK, WHITE, { A1, H1 }));
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

    gamePhase = EGamePhase::OPENNING;
}

Board::FigureIter Board::GetFigureIter(EColor color, EFigure figure, ESquare square)
{
    auto& list = figures[color][figure];
    auto iter = std::find_if(list.begin(), list.end(), [&](const auto& figure) {
        return figure->GetSquare() == square;
        });

    return iter;
}

Board::FigureIterConst Board::GetFigureIter(EColor color, EFigure figure, ESquare square) const
{
    const auto& list = figures[color][figure];
    auto iter = std::find_if(list.begin(), list.end(), [&](const auto& figure) {
        return figure->GetSquare() == square;
    });

    return iter;
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

unsigned int Board::GetFigureCount(EColor color, EFigure figureName)
{
    return figures[color][figureName].size();
}

unsigned int Board::GetSideFiguresCount(EColor color)
{
    int count = 0;
    for (int figureInt = 0; figureInt < EFigure::COUNT; ++figureInt) {
        count += GetFigureCount(color, static_cast<EFigure>(figureInt));
    }
    return count;
}

unsigned int Board::GetFigureSumMobility(EColor color, EFigure figureName)
{
    EColor oppositeColor = (color == WHITE) ? BLACK : WHITE;
    std::uint64_t blockers = GetSideBoard(color);
    std::uint64_t opposite = GetSideBoard(oppositeColor);

    int mobility = 0;
    for (auto it = figures[color][figureName].begin(); it != figures[color][figureName].end(); ++it)
    {
        std::uint64_t silentMoves = (*it)->GetSilentMoves(blockers, opposite);
        mobility += std::bitset<64>(silentMoves).count();
    }
    return mobility;
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

bool Board::IsShortCastlingPossible(EColor color) const
{
    U64 castlingBlockers = (color == WHITE) ?
        bitboards::castling_blockers::SHORT_CASTLING_WHITE :
        bitboards::castling_blockers::SHORT_CASTLING_BLACK;

    ESquare rookSquare = (color == WHITE) ? 
        positions_square::WHITE_RSH_ROOK :
        positions_square::BLACK_RSH_ROOK;

    return IsCastlingPossible_(color, castlingBlockers, rookSquare);
}

bool Board::IsLongCastlingPossible(EColor color) const
{
    U64 castlingBlockers = (color == WHITE) ?
        bitboards::castling_blockers::LONG_CASTLING_WHITE :
        bitboards::castling_blockers::LONG_CASTLING_BLACK;

    ESquare rookSquare = (color == WHITE) ?
        positions_square::WHITE_LSH_ROOK :
        positions_square::BLACK_LSH_ROOK;

    return IsCastlingPossible_(color, castlingBlockers, rookSquare);
}

bool Board::IsMoveLegal(const Move& move) const
{
    const_cast<Board*>(this)->makeMove(move);
    bool isLegal = !IsKingAttacked(move.GetMoveColor());
    const_cast<Board*>(this)->undoMove();

    return isLegal;
}

bool Board::IsGameOver() const
{
    return figures[WHITE][KING].empty() || figures[BLACK][KING].empty();
}

MoveList Board::GenerateMoveList(const std::unique_ptr<Figure>& figure) const
{
    MoveList moveList;

    EColor color = figure->GetColor();
    EColor oppositeColor = (color == WHITE) ? BLACK : WHITE;
    EFigure figureName = figure->GetFigureName();
    U64 fromSquare = figure->GetSquare();

    U64 blockers = GetSideBoard(color);
    U64 opposite = GetSideBoard(oppositeColor);

    U64 movesBoard = figure->GetMoves(blockers, opposite);

    U64 movesBoardCaptures = movesBoard & opposite;
    while (movesBoardCaptures) {
        int toSquare = misc::BitScanForward(movesBoardCaptures);
        int captureInt = figureFromCoord[oppositeColor][toSquare];
        Move move(fromSquare, toSquare, figureName, captureInt, EMoveType::CAPTURE, color);
        if (IsMoveLegal(move)) {
            moveList.add(move);
        }
        movesBoardCaptures &= movesBoardCaptures - 1;
    }

    U64 movesBoardSilents = movesBoard & ~opposite & ~blockers;
    while (movesBoardSilents) {
        int toSquare = misc::BitScanForward(movesBoardSilents);
        Move move(fromSquare, toSquare, figureName, EFigure::NO_FIGURE, EMoveType::SILENT, color);
        if (IsMoveLegal(move)) {
            moveList.add(move);
        }
        movesBoardSilents &= movesBoardSilents - 1;
    }

    return moveList;
}

MoveList Board::GenerateMoveList(EColor color, bool isCaptureOnly) const
{
    MoveList moveList;
    for (unsigned figureInt = 0; figureInt < EFigure::COUNT; ++figureInt) {
        const auto& list = figures[color][figureInt];
        for (auto figureIter = list.begin(); figureIter != list.end(); ++figureIter) {
            if (isCaptureOnly) {
                moveList += GenerateCaptureMoveList(*figureIter);
            }
            else {
                moveList += GenerateMoveList(*figureIter);
            }
        }
    }

    if (!isCaptureOnly) {
        if (IsShortCastlingPossible(color)) {
            moveList.add((color == WHITE) ? Move::GetWhiteShortCastlingMove() : Move::GetBlackShortCastlingMove());
        }
        if (IsLongCastlingPossible(color)) {
            moveList.add((color == WHITE) ? Move::GetWhiteLongCastlingMove() : Move::GetBlackLongCastlingMove());
        }
    }

    return moveList;
}

MoveList Board::GenerateCaptureMoveList(const std::unique_ptr<Figure>& figure) const
{
    MoveList moveList;

    EColor color = figure->GetColor();
    EColor oppositeColor = (color == WHITE) ? BLACK : WHITE;
    EFigure figureName = figure->GetFigureName();
    U64 fromSquare = figure->GetSquare();

    U64 movesBoardCaptures = figure->GetCaptureMoves(GetSideBoard(color), GetSideBoard(oppositeColor));

    while (movesBoardCaptures) {
        int toSquare = misc::BitScanForward(movesBoardCaptures);
        int captureInt = figureFromCoord[oppositeColor][toSquare];
        Move move(fromSquare, toSquare, figureName, captureInt, EMoveType::CAPTURE, color);
        if (IsMoveLegal(move)) {
            moveList.add(move);
        }
        movesBoardCaptures &= movesBoardCaptures - 1;
    }

    return moveList;
}

void Board::makeMove(const Move& move)
{
    if (move.GetCapture() != EFigure::NO_FIGURE) {
        RemoveCapture_(move);
    }

    //auto figureIter = GetFigureIter(move.GetMoveColor(), move.GetFigure(), move.GetFrom());
    //if (figureIter == figures[move.GetMoveColor()][move.GetFigure()].end())
    //{
    //    std::cout << "1!!!!!!-----------";
    //}
    //(*figureIter)->move(move.GetTo());
    //figureFromCoord[move.GetMoveColor()][move.GetFrom()] = EFigure::NO_FIGURE;
    //figureFromCoord[move.GetMoveColor()][move.GetTo()] = move.GetFigure();
    MoveFigure_(move.GetFigure(), move.GetMoveColor(), move.GetFrom(), move.GetTo());

    switch (move.GetMoveType()) {
    case EMoveType::SHORT_CASTLING:
        MakeShortCastling_(move.GetMoveColor());
        break;
    case EMoveType::LONG_CASTLING:
        MakeLongCastling_(move.GetMoveColor());
        break;
    }

    madedMoves.push_back(move);
}

void Board::undoMove()
{
    assert(!madedMoves.empty());

    const Move& lastMove = madedMoves.back();

    if (lastMove.GetCapture() != EFigure::NO_FIGURE) {
        RestoreCapture_();
    }

    //auto figureIter = GetFigureIter(lastMove.GetMoveColor(), lastMove.GetFigure(), lastMove.GetTo());
    //(*figureIter)->moveBack();
    //figureFromCoord[lastMove.GetMoveColor()][lastMove.GetTo()] = EFigure::NO_FIGURE;
    //figureFromCoord[lastMove.GetMoveColor()][lastMove.GetFrom()] = lastMove.GetFigure();
    MoveBackFigure_(lastMove.GetFigure(), lastMove.GetMoveColor(), lastMove.GetFrom(), lastMove.GetTo());

    switch (lastMove.GetMoveType()) {
    case EMoveType::SHORT_CASTLING:
        UndoShortCastling_(lastMove.GetMoveColor());
        break;
    case EMoveType::LONG_CASTLING:
        UndoLongCastling_(lastMove.GetMoveColor());
        break;
    }

    madedMoves.pop_back();
}

void Board::RemoveCapture_(const Move& move)
{
    EColor captureColor = (move.GetMoveColor() == EColor::WHITE) ? EColor::BLACK : EColor::WHITE;
    auto captureIter = GetFigureIter(captureColor, move.GetCapture(), move.GetTo());

    figureCache.push(std::move(*captureIter));
    figures[captureColor][move.GetCapture()].erase(captureIter);
    figureFromCoord[captureColor][move.GetTo()] = EFigure::NO_FIGURE;
}

void Board::RestoreCapture_()
{
    assert(!figureCache.empty());

    EColor captureColor = figureCache.top()->GetColor();
    EFigure captureFigure = figureCache.top()->GetFigureName();
    ESquare square = figureCache.top()->GetSquare();

    figures[captureColor][captureFigure].push_back(std::move(figureCache.top()));
    figureCache.pop();
    figureFromCoord[captureColor][square] = captureFigure;
}

bool Board::IsCastlingPossible_(EColor color, const U64& castlingBlockers, ESquare rookSquare) const
{
    if (IsKingAttacked(color)) {
        return false;
    }

    auto rookIter = GetFigureIter(color, ROOK, rookSquare);
    if (rookIter == figures[color][ROOK].end()) {
        return false;
    }

    auto kingIter = figures[color][KING].begin();
    if ((*rookIter)->GetMovesCount() > 0 || (*kingIter)->GetMovesCount() > 0) {
        return false;
    }

    EColor oppositeColor = (color == WHITE) ? BLACK : WHITE;
    U64 isBlockersBetweenKingAndRook = GetSideBoard(color) & castlingBlockers;
    U64 isCastlingUnderAttack = GetAttackRays(oppositeColor) & castlingBlockers;
    
    return !(isBlockersBetweenKingAndRook || isCastlingUnderAttack);
}

void Board::MakeShortCastling_(EColor color)
{
    ESquare from = (color == WHITE) ? positions_square::WHITE_RSH_ROOK : positions_square::BLACK_RSH_ROOK;
    ESquare to = (color == WHITE) ? positions_square::WHITE_RSH_ROOK_CASTLED : positions_square::BLACK_RSH_ROOK_CASTLED;
    MoveFigure_(ROOK, color, from, to);
}

void Board::MakeLongCastling_(EColor color)
{
    ESquare from = (color == WHITE) ? positions_square::WHITE_LSH_ROOK : positions_square::BLACK_LSH_ROOK;
    ESquare to = (color == WHITE) ? positions_square::WHITE_LSH_ROOK_CASTLED : positions_square::BLACK_LSH_ROOK_CASTLED;
    MoveFigure_(ROOK, color, from, to);
}

void Board::UndoShortCastling_(EColor color)
{
    ESquare from = (color == WHITE) ? positions_square::WHITE_RSH_ROOK : positions_square::BLACK_RSH_ROOK;
    ESquare to = (color == WHITE) ? positions_square::WHITE_RSH_ROOK_CASTLED : positions_square::BLACK_RSH_ROOK_CASTLED;
    MoveBackFigure_(ROOK, color, from, to);
}

void Board::UndoLongCastling_(EColor color)
{
    ESquare from = (color == WHITE) ? positions_square::WHITE_LSH_ROOK : positions_square::BLACK_LSH_ROOK;
    ESquare to = (color == WHITE) ? positions_square::WHITE_LSH_ROOK_CASTLED : positions_square::BLACK_LSH_ROOK_CASTLED;
    MoveBackFigure_(ROOK, color, from, to);
}

void Board::MoveFigure_(EFigure figureName, EColor color, ESquare from, ESquare to)
{
    auto figureIter = GetFigureIter(color, figureName, from);
    (*figureIter)->move(to);
    figureFromCoord[color][from] = EFigure::NO_FIGURE;
    figureFromCoord[color][to] = figureName;
}

void Board::MoveBackFigure_(EFigure figureName, EColor color, ESquare from, ESquare to)
{
    auto figureIter = GetFigureIter(color, figureName, to);
    (*figureIter)->moveBack();
    figureFromCoord[color][to] = EFigure::NO_FIGURE;
    figureFromCoord[color][from] = figureName;
}
