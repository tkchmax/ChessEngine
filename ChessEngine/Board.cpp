#include "Board.h"
#include <cassert>
#include <iostream>
#include <bitset>

namespace
{
    MoveList GetPawnTransformationMoves(int fromInt, int toInt, int captureInt, int colorInt)
    {
        MoveList moveList;
        for (int transformType = EMoveType::PAWN_TO_KNIGHT; transformType <= EMoveType::PAWN_TO_QUEEN; ++transformType) {
            moveList.add(Move(fromInt, toInt, PAWN, captureInt, transformType, colorInt));
        }
        return moveList;
    }
}

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

void Board::Clear()
{
    for (int colorInt = 0; colorInt < 2; ++colorInt) {
        for (int figureInt = 0; figureInt < 6; ++figureInt) {
            figures[colorInt][figureInt].clear();
        }
        for (int i = 0; i < 64; ++i) {
            figureFromCoord[colorInt][i] = NO_FIGURE;
        }
    }
}

void Board::SetFEN(std::string fen)
{
    Clear();

    int board_cur = 72;
    int cur = -1;
    for (int i = 0; i < 8; ++i) {
        board_cur -= 16;
        cur++;
        while (fen[cur] != '/')
        {
            ESquare square = static_cast<ESquare>(board_cur);
            switch (fen[cur])
            {
            case 'K':
                figures[WHITE][KING].push_back(std::make_unique<King>(WHITE, square));
                figureFromCoord[WHITE][square] = KING;
                board_cur++;
                break;
            case 'k':
                figures[BLACK][KING].push_back(std::make_unique<King>(BLACK, square));
                figureFromCoord[BLACK][square] = KING;
                board_cur++;
                break;
            case 'Q':
                figures[WHITE][QUEEN].push_back(std::make_unique<Queen>(WHITE, square));
                figureFromCoord[WHITE][square] = QUEEN;
                board_cur++;
                break;
            case 'q':
                figures[BLACK][QUEEN].push_back(std::make_unique<Queen>(BLACK, square));
                figureFromCoord[BLACK][square] = QUEEN;
                board_cur++;
                break;
            case 'B':
                figures[WHITE][BISHOP].push_back(std::make_unique<Bishop>(WHITE, square));
                figureFromCoord[WHITE][square] = BISHOP;
                board_cur++;
                break;
            case 'b':
                figures[BLACK][BISHOP].push_back(std::make_unique<Bishop>(BLACK, square));
                figureFromCoord[BLACK][square] = BISHOP;
                board_cur++;
                break;
            case 'N':
                figures[WHITE][KNIGHT].push_back(std::make_unique<Knight>(WHITE, square));
                figureFromCoord[WHITE][square] = KNIGHT;
                board_cur++;
                break;
            case 'n':
                figures[BLACK][KNIGHT].push_back(std::make_unique<Knight>(BLACK, square));
                figureFromCoord[BLACK][square] = KNIGHT;
                board_cur++;
                break;
            case 'P':
                figures[WHITE][PAWN].push_back(std::make_unique<Pawn>(WHITE, square));
                figureFromCoord[WHITE][square] = PAWN;
                board_cur++;
                break;
            case 'p':
                figures[BLACK][PAWN].push_back(std::make_unique<Pawn>(BLACK, square));
                figureFromCoord[BLACK][square] = PAWN;
                board_cur++;
                break;
            case 'R':
                figures[WHITE][ROOK].push_back(std::make_unique<Rook>(WHITE, square));
                figureFromCoord[WHITE][square] = ROOK;
                board_cur++;
                break;
            case 'r':
                figures[BLACK][ROOK].push_back(std::make_unique<Rook>(BLACK, square));
                figureFromCoord[BLACK][square] = ROOK;
                board_cur++;
                break;
            default:
                board_cur += (fen[cur] - '0');
                break;
            }
            cur++;
        }
    }
}

std::string Board::GetPGN()
{
    std::string PGN = "";
    std::unique_ptr<Board> tmpBoard(new Board());

    int i = 0;
    for (auto moveIter = madedMoves.begin(); moveIter != madedMoves.end(); ++moveIter, ++i) {
        if (i % 2 == 0) {
            PGN += std::to_string(i) + ". ";
        }
        PGN += (*moveIter).GetNotation(tmpBoard->IsExpandedNotationNeeded(*moveIter)) + " ";
        tmpBoard->makeMove(*moveIter);
    }
    return PGN;
}

bool Board::IsKingAttacked(EColor color) const
{
    U64 attackRays = GetAttackRays((color == WHITE) ? BLACK : WHITE);
    auto kingIter = figures[color][KING].begin();

    if (kingIter == figures[color][KING].end()) {
        return true;
    }
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

MoveList Board::GenerateMoveList_(U64 bitboard, EFigure figureName, ESquare fromSquare, EColor color) const
{
    MoveList moveList;
    EColor oppositeColor = (color == WHITE) ? BLACK : WHITE;
    U64 pawnTransformLine = (color == WHITE) ? ~bitboards::NOT_8_RANK : ~bitboards::NOT_1_RANK;
    while (bitboard) {
        int toSquare = misc::BitScanForward(bitboard);
        int captureInt = figureFromCoord[oppositeColor][toSquare];
        int moveTypeInt = (captureInt == EFigure::NO_FIGURE) ? EMoveType::SILENT : EMoveType::CAPTURE;
        Move move(fromSquare, toSquare, figureName, captureInt, moveTypeInt, color);
        if (IsMoveLegal(move)) {
            bool isPawnTransformMove = figureName == EFigure::PAWN && (TO_BITBOARD(toSquare) & pawnTransformLine);
            if (isPawnTransformMove) {
                moveList += GetPawnTransformationMoves(fromSquare, toSquare, captureInt, color);
            }
            else {
                moveList.add(move);
            }
        }
        bitboard &= bitboard - 1;
    }
    return moveList;
}

MoveList Board::GenerateMoveList_(const std::unique_ptr<Figure>& figure, bool isCaptureOnly) const
{
    MoveList moveList;

    EColor color = figure->GetColor();
    EColor oppositeColor = (color == WHITE) ? BLACK : WHITE;
    EFigure figureName = figure->GetFigureName();
    ESquare fromSquare = figure->GetSquare();

    U64 blockers = GetSideBoard(color);
    U64 opposite = GetSideBoard(oppositeColor);
    U64 movesBoard = figure->GetMoves(blockers, opposite);

    U64 movesBoardCaptures = movesBoard & opposite;
    moveList += GenerateMoveList_(movesBoardCaptures, figureName, fromSquare, color);

    if (!isCaptureOnly) {
        U64 movesBoardSilents = movesBoard & ~opposite & ~blockers;
        moveList += GenerateMoveList_(movesBoardSilents, figureName, fromSquare, color);
    }
    return moveList;
}

MoveList Board::GenerateMoveList(EColor color, bool isCaptureOnly) const
{
    MoveList moveList;

    if (!isCaptureOnly) {
        if (IsShortCastlingPossible(color)) {
            moveList.add((color == WHITE) ? Move::GetWhiteShortCastlingMove() : Move::GetBlackShortCastlingMove());
        }
        if (IsLongCastlingPossible(color)) {
            moveList.add((color == WHITE) ? Move::GetWhiteLongCastlingMove() : Move::GetBlackLongCastlingMove());
        }
    }

    for (unsigned figureInt = 0; figureInt < EFigure::COUNT; ++figureInt) {
        const auto& list = figures[color][figureInt];
        for (auto figureIter = list.begin(); figureIter != list.end(); ++figureIter) {
            moveList += GenerateMoveList_(*figureIter, isCaptureOnly);
        }
    }

    return moveList;
}

void Board::makeMove(const Move& move)
{
    if (move.Get() == 0) { return; } //!!!
    madedMoves.push_back(move);

    if (move.GetCapture() != EFigure::NO_FIGURE) {
        RemoveCapture_(move);
    }

    if (IsPawnTransformType(move.GetMoveType())) {
        MakePawnTransformMove_(move);
        return;
    }

    MoveFigure_(move.GetFigure(), move.GetMoveColor(), move.GetFrom(), move.GetTo());

    switch (move.GetMoveType()) {
    case EMoveType::SHORT_CASTLING:
        MakeShortCastling_(move.GetMoveColor());
        break;
    case EMoveType::LONG_CASTLING:
        MakeLongCastling_(move.GetMoveColor());
        break;
    }
}

void Board::undoMove()
{
    assert(!madedMoves.empty());

    Move lastMove = madedMoves.back();

    if (lastMove.GetCapture() != EFigure::NO_FIGURE) {
        RestoreCapture_();
    }

    if (IsPawnTransformType(lastMove.GetMoveType())) {
        UndoPawnTransformMove_();
        madedMoves.pop_back();
        return;
    }

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

    captureStack.push(std::move(*captureIter));
    figures[captureColor][move.GetCapture()].erase(captureIter);
    figureFromCoord[captureColor][move.GetTo()] = EFigure::NO_FIGURE;
}

void Board::RestoreCapture_()
{
    assert(!captureStack.empty());

    EColor captureColor = captureStack.top()->GetColor();
    EFigure captureFigure = captureStack.top()->GetFigureName();
    ESquare square = captureStack.top()->GetSquare();

    figures[captureColor][captureFigure].push_back(std::move(captureStack.top()));
    captureStack.pop();
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
    U64 isBlockersBetweenKingAndRook = (GetSideBoard(color) | GetSideBoard (oppositeColor)) & castlingBlockers;
    U64 isCastlingUnderAttack = GetAttackRays(oppositeColor) & castlingBlockers;

    return !(isBlockersBetweenKingAndRook || isCastlingUnderAttack);
}

bool Board::IsExpandedNotationNeeded(const Move& move)
{
    MoveList moveList = GenerateMoveList(move.GetMoveColor());

    for (auto moveIter = moveList.Get().begin(); moveIter != moveList.Get().end(); moveIter++) {
        const Move& m = *moveIter;
        bool isAnotherFigureGoesOnSameSquare = m.GetFigure() == move.GetFigure() &&
            m.GetTo() == move.GetTo() && m.GetFrom() != move.GetFrom();
        if (isAnotherFigureGoesOnSameSquare) {
            return true;
        }
    }
    return false;
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

void Board::MakePawnTransformMove_(const Move& move)
{
    EColor color = move.GetMoveColor();
    ESquare from = move.GetFrom();
    ESquare to = move.GetTo();

    auto pawnIter = GetFigureIter(color, PAWN, from);
    assert(pawnIter != figures[color][PAWN].end());

    pawnTransformStack.push(std::move(*pawnIter));
    figures[color][PAWN].erase(pawnIter);
    figureFromCoord[color][from] = NO_FIGURE;

    EFigure transformTo;
    switch (move.GetMoveType()) {
    case EMoveType::PAWN_TO_KNIGHT:
        transformTo = EFigure::KNIGHT;
        break;
    case EMoveType::PAWN_TO_BISHOP:
        transformTo = EFigure::BISHOP;
        break;
    case EMoveType::PAWN_TO_ROOK:
        transformTo = EFigure::ROOK;
        break;
    case EMoveType::PAWN_TO_QUEEN:
        transformTo = EFigure::QUEEN;
        break;
    default: 
        assert(false);
        break;
    }
    figures[color][transformTo].push_back(std::move(Figure::Create(transformTo, color, to)));
    figureFromCoord[color][to] = transformTo;
}

void Board::UndoPawnTransformMove_()
{
    const Move& transformMove = madedMoves.back();

    EColor color = transformMove.GetMoveColor();
    ESquare from = transformMove.GetFrom();
    ESquare to = transformMove.GetTo();

    EFigure transformTo;
    switch (transformMove.GetMoveType()) {
    case EMoveType::PAWN_TO_KNIGHT:
        transformTo = EFigure::KNIGHT;
        break;
    case EMoveType::PAWN_TO_BISHOP:
        transformTo = EFigure::BISHOP;
        break;
    case EMoveType::PAWN_TO_ROOK:
        transformTo = EFigure::ROOK;
        break;
    case EMoveType::PAWN_TO_QUEEN:
        transformTo = EFigure::QUEEN;
        break;
    default:
        assert(false);
        break;
    }

    auto figureIter = GetFigureIter(color, transformTo, to);
    assert(figureIter != figures[color][transformTo].end());

    figures[color][transformTo].erase(figureIter);
    figureFromCoord[color][to] = NO_FIGURE;

    figures[color][PAWN].push_back(std::move(pawnTransformStack.top()));
    pawnTransformStack.pop();
    figureFromCoord[color][from] = PAWN;
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
    if (figureIter == figures[color][figureName].end())
    {
        std::cout << color << std::endl;
        std::cout << from << std::endl;
        std::cout << to << std::endl;
    }
    assert(figureIter != figures[color][figureName].end());
    (*figureIter)->move(to);
    figureFromCoord[color][from] = EFigure::NO_FIGURE;
    figureFromCoord[color][to] = figureName;
}

void Board::MoveBackFigure_(EFigure figureName, EColor color, ESquare from, ESquare to)
{
    auto figureIter = GetFigureIter(color, figureName, to);
    assert(figureIter != figures[color][figureName].end());
    (*figureIter)->moveBack();
    figureFromCoord[color][to] = EFigure::NO_FIGURE;
    figureFromCoord[color][from] = figureName;
}
