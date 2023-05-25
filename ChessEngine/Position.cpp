#include "Position.h"
#include "Misc.h"
#include "Magic.h"
#include "Rays.h"
#include "Transposition.h"
#include <stdexcept>
#include <cassert>
#include <iomanip>


namespace {
    constexpr char figureSymbol[12] = { 'P','N','B','R','Q','K','p','n','b','r','q','k' };

    constexpr int MVV_LVA_BONUS[7][7] = {
        {0,0,0,0,0,0,0},
        {0,1500,2500,3500,4500,5500,6500},
        {0,1300,2300,3300,4300,5300,6300},
        {0,1100,2100,3100,4100,5100,6100},
        {0,900,1900,2900,3900,4900,5900},
        {0,700,1700,2700,3700,4700,5700},
        {0,500,1500,2500,3500,4500,5500},
    };

    template<EColor color>
    U64 GetPawnAttackBB(U64 pawnsBB) {
        return color == WHITE ?
            misc::shift_bb<NORTH_EAST>(pawnsBB) | misc::shift_bb<NORTH_WEST>(pawnsBB) :
            misc::shift_bb<SOUTH_EAST>(pawnsBB) | misc::shift_bb<SOUTH_WEST>(pawnsBB);
    }

    template<EGenType gentype, EDir dir>
    void MakePromotions(MoveList& list, ESquare to) {
        const ESquare from = ESquare(to - misc::shift_value<dir>());
        list.add(EncodeMove<PROMOTION>(from, to, KNIGHT));
        list.add(EncodeMove<PROMOTION>(from, to, QUEEN));
    }

    template<EColor color, EGenType gentype>
    void GeneratePawnMoves(const Position& pos, MoveList& list)
    {
        using namespace misc;

        constexpr EColor opColor = ~color;
        constexpr EDir upRight = (color == WHITE) ? NORTH_EAST : SOUTH_WEST;
        constexpr EDir upLeft = (color == WHITE) ? NORTH_WEST : SOUTH_EAST;
        constexpr EDir up = (color == WHITE) ? NORTH : SOUTH;
        constexpr U64 transformLine = (color == WHITE) ? bitboards::RANK_7 : bitboards::RANK_2;
        constexpr U64 doublePushLine = (color == WHITE) ? bitboards::RANK_4 : bitboards::RANK_5;

        const U64 occ = pos.figures(ALL_FIGURE_TYPE);
        const U64 pawns_bb = pos.figures(color, PAWN);
        const U64 transformPawns = pawns_bb & transformLine;

        //Capture Moves
        if constexpr (gentype != EGenType::QUIETS) {
            //Right pawns attack
            U64 rightHook = shift_bb<upRight>(pawns_bb & ~transformLine);
            rightHook &= pos.figures(opColor);
            while (rightHook) {
                ESquare to = pop_lsb(rightHook);
                ESquare from = ESquare(to - shift_value<upRight>());
                list.add(EncodeMove<NORMAL>(from, to, PAWN), MVV_LVA_BONUS[PAWN][type_of(pos.on_square(to))]);
            }

            //Left pawns attack
            U64 leftHook = shift_bb<upLeft>(pawns_bb & ~transformLine);
            leftHook &= pos.figures(opColor);
            while (leftHook) {
                ESquare to = pop_lsb(leftHook);
                ESquare from = ESquare(to - shift_value<upLeft>());
                list.add(EncodeMove<NORMAL>(from, to, PAWN), MVV_LVA_BONUS[PAWN][type_of(pos.on_square(to))]);
            }

            //EnPassant
            constexpr U64 epLine = (color == WHITE) ? bitboards::RANK_5 : bitboards::RANK_4;
            if (pos.get_en_passant() != SQ_NONE) {
                const U64 epBB = TO_BITBOARD(pos.get_en_passant());
                U64 epCandidates =
                    pawns_bb & (shift_bb<EAST>(epBB) | shift_bb<WEST>(epBB));

                ESquare to = ESquare(lsb(shift_bb<up>(epBB)));
                while (epCandidates) {
                    ESquare from = pop_lsb(epCandidates);
                    list.add(EncodeMove<EN_PASSANT>(from, ESquare(lsb(shift_bb<up>(epBB))), PAWN), MVV_LVA_BONUS[PAWN][PAWN]);
                }
            }

            // Capture promotions
            if (transformPawns) {
                U64 prom2 = shift_bb<upRight>(transformPawns) & pos.figures(opColor);
                U64 prom3 = shift_bb<upLeft>(transformPawns) & pos.figures(opColor);
                while (prom2) {
                    MakePromotions<gentype, upRight>(list, pop_lsb(prom2));
                }
                while (prom3) {
                    MakePromotions<gentype, upLeft>(list, pop_lsb(prom3));
                }
            }
        }

        //Quiet Moves
        if constexpr (gentype != CAPTURES) {
            //One-Push Moves
            U64 onePushBoard = shift_bb<up>(pawns_bb & ~transformLine);
            onePushBoard &= ~occ;

            U64 doublePushBoard = onePushBoard;
            while (onePushBoard) {
                ESquare to = pop_lsb(onePushBoard);
                ESquare from = ESquare(to - shift_value<up>());
                list.add(EncodeMove<NORMAL>(from, to, PAWN));
            }

            //Double-Push Moves
            doublePushBoard = shift_bb<up>(doublePushBoard);
            doublePushBoard &= doublePushLine & ~occ;

            while (doublePushBoard) {
                ESquare to = pop_lsb(doublePushBoard);
                ESquare from = ESquare(to - 2 * shift_value<up>());
                list.add(EncodeMove<DOUBLE_PUSH>(from, to, PAWN));
            }

            // Quiet promotions
            if (transformPawns) {
                U64 prom1 = shift_bb<up>(transformPawns) & ~occ;
                while (prom1) {
                    MakePromotions<gentype, up>(list, pop_lsb(prom1));
                }
            }
        }
    }

    template<EColor color, EFigureType fig_type>
    void GenerateMove(const Position& pos, MoveList& list, U64 target_squares) {
        assert(fig_type != PAWN);

        U64 figuresBB = pos.figures(color, fig_type);

        while (figuresBB) {
            ESquare from = misc::pop_lsb(figuresBB);
            U64 attacks = GetAttackBB<fig_type>(from, pos.figures(EFigureType::ALL_FIGURE_TYPE)) & target_squares;
            while (attacks) {
                ESquare to = misc::pop_lsb(attacks);
                list.add(EncodeMove<NORMAL>(from, to, fig_type), MVV_LVA_BONUS[fig_type][type_of(pos.on_square(to))]);
            }
        }
    }

    template<EColor color, EGenType gentype>
    void GenerateAll(const Position& pos, MoveList& list) {

        constexpr EColor opColor = ~color;

        //Mask to use with attack bitboard of each figure
        U64 targetSq = gentype == QUIETS ? ~pos.figures(opColor) :
            gentype == CAPTURES ? pos.figures(opColor) :
            ~pos.figures(color) | pos.figures(opColor);

        GeneratePawnMoves<color, gentype>(pos, list);
        GenerateMove<color, KNIGHT>(pos, list, targetSq);
        GenerateMove<color, BISHOP>(pos, list, targetSq);
        GenerateMove<color, ROOK>(pos, list, targetSq);
        GenerateMove<color, QUEEN>(pos, list, targetSq);
        GenerateMove<color, KING>(pos, list, targetSq);

        //castling
        if constexpr (gentype != CAPTURES) {
            for (CastlingRights cr : castling::byColor[color]) {
                bool rookExist = TO_BITBOARD(castling::rookSquare[cr]) & pos.figures(color, ROOK);
                if (pos.can_castling(cr) && !pos.is_castling_impeded(cr) && rookExist) {
                    ESquare ksq = color == WHITE ? E1 : E8;

                    if (!(pos.attack_to(ksq) & pos.figures(opColor))) {
                        if (!(pos.attack_to(castling::kingSafetySq[cr]) & pos.figures(opColor))) {
                            list.add(EncodeMove<CASTLING>(ksq, castling::rookSquare[cr], KING));
                        }
                    }
                }
            }
        }
    }
}

Position::Position(std::string fen)
{
    this->setFEN(fen);
}

EGamePhase Position::calculate_phase() const
{
    int gamePhaseScore = calculate_phase_score();

    EGamePhase gp =
        gamePhaseScore > evaluate::opening_bounder_score ? EGamePhase::OPENNING :
        gamePhaseScore < evaluate::endgame_bounder_score ? EGamePhase::END_GAME :
        EGamePhase::MIDDLE_GAME;

    return gp;
}

int Position::phase_score() const
{
    return gpScore;
}

void Position::setFEN(const std::string& fen)
{
    attackedSquares = 0;
    enPassant = ESquare::SQ_NONE;

    //Clear representation of the board
    for (int fig = W_PAWN; fig <= B_KING; ++fig) {
        figureCount[fig] = 0;
    }
    for (int color = 0; color < COLOR_NB; ++color) {
        byColorBB[color] = 0ULL;
    }
    for (int figType = 0; figType < FIGURE_TYPE_NB; ++figType) {
        byTypeBB[figType] = 0ULL;
    }
    for (int i = 0; i < 64; ++i) {
        board[i] = NO_FIGURE;
    }

    int squareInt = ESquare::A8;
    int cur = 0;
    for (; fen[cur] != ' '; ++cur) {
        if ((int)fen[cur] >= '1' && (int)fen[cur] <= '8') {
            squareInt += fen[cur] - '0';
        }
        else if (fen[cur] == '/') {
            squareInt -= 16;
        }
        else
        {
            EFigureType type = NO_FIGURE_TYPE;
            EColor color = WHITE;
            switch (fen[cur])
            {
            case 'K':
                type = KING;
                break;
            case 'k':
                type = KING;
                color = BLACK;
                break;
            case 'Q':
                type = QUEEN;
                break;
            case 'q':
                type = QUEEN;
                color = BLACK;
                break;
            case 'B':
                type = BISHOP;
                break;
            case 'b':
                type = BISHOP;
                color = BLACK;
                break;
            case 'N':
                type = KNIGHT;
                break;
            case 'n':
                type = KNIGHT;
                color = BLACK;
                break;
            case 'P':
                type = PAWN;
                break;
            case 'p':
                type = PAWN;
                color = BLACK;
                break;
            case 'R':
                type = ROOK;
                break;
            case 'r':
                type = ROOK;
                color = BLACK;
                break;
            }

            board[squareInt] = add_color(color, type);
            byTypeBB[type] |= TO_BITBOARD(squareInt);
            byTypeBB[ALL_FIGURE_TYPE] |= TO_BITBOARD(squareInt);
            byColorBB[color] |= TO_BITBOARD(squareInt);
            figureCount[board[squareInt]]++;
            squareInt++;
        }
    }

    // w or b
    if (++cur < fen.size()) {
        char side = fen[cur++];
        if (side == 'w') {
            sideToMove = EColor::WHITE;
        }
        else if (side == 'b') {
            sideToMove = EColor::BLACK;
        }
        else {
            throw std::invalid_argument("Invalid FEN");
        }
    }

    castlingRights = 0;
    for (cur++; fen[cur] != ' ' && cur < fen.size(); ++cur) {
        switch (fen[cur]) {
        case 'K':
            castlingRights |= WHITE_00;
            break;
        case 'k':
            castlingRights |= BLACK_00;
            break;
        case 'Q':
            castlingRights |= WHITE_000;
            break;
        case 'q':
            castlingRights |= BLACK_000;
            break;
        }
    }

    char epFile = fen[++cur];
    if (epFile != '-') {
        char epRank = fen[++cur];
        enPassant = ESquare(8 * (epRank - '1') + epFile - 'a');
        std::cout << "";
    }

    gp = calculate_phase();
    gpScore = calculate_phase_score();

    zobrist = Transposition::GetZobristHash(*this);
}

std::ostream& operator<<(std::ostream& out, const Position& pos)
{
    out << "+---+---+---+---+---+---+---+---+\n";
    for (int sq = A8; sq >= 0; ++sq) {

        EFigure fig = pos.board[sq];
        char symbol = (fig == NO_FIGURE) ? ' ' : figureSymbol[(color_of(fig) == WHITE) ? type_of(fig) - 1 : type_of(fig) + 5];
        out << "| " << symbol << " ";

        if ((sq + 1) % 8 == 0) {
            out << "| " << (sq / 8) + 1 << std::endl;
            out << "+---+---+---+---+---+---+---+---+\n";
            sq -= 16;
        }
    }
    out << "  a   b   c   d   e   f   g   h\n";

    out << std::endl;

    out << std::setw(15) << "Side: " << std::setw(5) << misc::ToString(pos.side_to_move()) << std::endl;
    out << std::setw(15) << "Enpassant: " << std::setw(5) << misc::ToString(pos.get_en_passant()) << std::endl;

    std::string castling(4, ' ');
    int cr = pos.get_castling_rights();
    castling[0] = (cr & WHITE_00) ? 'K' : '-';
    castling[1] = (cr & WHITE_000) ? 'Q' : '-';
    castling[2] = (cr & BLACK_00) ? 'k' : '-';
    castling[3] = (cr & BLACK_000) ? 'q' : '-';

    out << std::setw(15) << "Castling: " << std::setw(5) << castling << std::endl << std::endl;
    return out;
}

U64 Position::attack_to(ESquare square) const {
    return
        (GetPawnAttackBB<WHITE>(TO_BITBOARD(square)) & figures(BLACK, PAWN)) |
        (GetPawnAttackBB<BLACK>(TO_BITBOARD(square)) & figures(WHITE, PAWN)) |
        (GetAttackBB<KNIGHT>(square) & byTypeBB[KNIGHT]) |
        //(GetAttackBB<BISHOP>(square, byTypeBB[ALL_FIGURE_TYPE] ^ figures(sideToMove, KING)) & (byTypeBB[BISHOP] | byTypeBB[QUEEN])) |
        (GetAttackBB<BISHOP>(square, byTypeBB[ALL_FIGURE_TYPE]) & (byTypeBB[BISHOP] | byTypeBB[QUEEN])) |
        //(GetAttackBB<ROOK>(square, byTypeBB[ALL_FIGURE_TYPE] ^ figures(sideToMove, KING)) & (byTypeBB[ROOK] | byTypeBB[QUEEN])) |
        (GetAttackBB<ROOK>(square, byTypeBB[ALL_FIGURE_TYPE]) & (byTypeBB[ROOK] | byTypeBB[QUEEN])) |
        (GetAttackBB<KING>(square) & byTypeBB[KING]);
}

bool Position::is_move_legal(Move move) const
{
    Position tempPos = make_move(move);
    return !tempPos.is_king_attacked(sideToMove);
}

Position Position::make_move(Move m) const
{
    ESquare from = ESquare(READ_FROM(m));
    ESquare to = ESquare(READ_TO(m));
    EFigureType fig = EFigureType(READ_FIGURE(m));
    EMoveType move_type = EMoveType(READ_MOVE_TYPE(m));

    EColor color = sideToMove;
    EColor opColor = ~color;
    EFigure capture = move_type == CASTLING ? NO_FIGURE : on_square(to);

    Position newPos(*this);
    newPos.enPassant = SQ_NONE;
    newPos.sideToMove = opColor;

    if (move_type == CASTLING) {
        newPos.make_castling(color, from, to);
    }
    else if (move_type == DOUBLE_PUSH) {
        newPos.enPassant = to;
        newPos.move_figure(from, to);
    }

    int gphase = newPos.gpScore;
    if (capture != NO_FIGURE) {
        newPos.remove_figure(to);

        //Update game-phase if needed
        if (type_of(capture) >= KNIGHT && type_of(capture) <= QUEEN) {
            newPos.gpScore -= evaluate::material_score[OPENNING][type_of(capture)];
            assert(newPos.calculate_phase_score() == newPos.gpScore);
        }
    }

    if (move_type == NORMAL) {
        newPos.move_figure(from, to);

        if (fig == KING) {
            //newPos.castling_rights &= ~(3 << (color * 2));
            newPos.castlingRights &= color == WHITE ? ~(WHITE_00 | WHITE_000) : ~(BLACK_00 | BLACK_000);
        }

        if (fig == ROOK) {
            if (from == castling::rookFromTo00[color][0]) {
                newPos.castlingRights &= color == WHITE ? ~WHITE_00 : ~BLACK_00;
            }
            else if (from == castling::rookFromTo000[color][0]) {
                newPos.castlingRights &= color == WHITE ? ~WHITE_000 : ~BLACK_000;
            }
        }
    }
    else if (move_type == PROMOTION) {
        newPos.remove_figure(from);
        newPos.create_figure(to, add_color(color, fig));

        //Update game phase score
        newPos.gpScore += evaluate::material_score[OPENNING][fig];
    }
    else if (move_type == EN_PASSANT) {
        newPos.remove_figure(ESquare(color == WHITE ? to - 8 : to + 8));
        newPos.move_figure(from, to);
    }

    assert(newPos.calculate_phase_score() == newPos.gpScore);

    //Set new game phase
    newPos.gp =
        newPos.gpScore > evaluate::opening_bounder_score ? EGamePhase::OPENNING :
        newPos.gpScore < evaluate::endgame_bounder_score ? EGamePhase::END_GAME :
        EGamePhase::MIDDLE_GAME;

    Transposition::AmmendHash(newPos.zobrist, m, *this);

    return newPos;
}

void Position::make_castling(EColor color, ESquare from, ESquare to)
{
    ESquare rf = castling::rookFromTo00[color][0];
    ESquare rto = castling::rookFromTo00[color][1];
    ESquare kto = castling::kingTo00[color];

    //if long castling
    if (from > to) {
        rf = castling::rookFromTo000[color][0];
        rto = castling::rookFromTo000[color][1];
        kto = castling::kingTo000[color];
    }

    //move king
    move_figure(from, kto);

    //move rook
    move_figure(rf, rto);

    //delete rights for castling
    castlingRights &= color == WHITE ? ~(WHITE_00 | WHITE_000) : ~(BLACK_00 | BLACK_000);
}

template<EGenType gentype>
void Position::generate(MoveList& list) const {

    sideToMove == WHITE ?
        GenerateAll<WHITE, gentype>(*this, list) :
        GenerateAll<BLACK, gentype>(*this, list);
}

template<>
void Position::generate<LEGAL>(MoveList& legalMoves) const {
    MoveList pseudoMoves;
    generate<PSEUDO>(pseudoMoves);

    for (int i = 0; i < pseudoMoves.size(); ++i) {
        if (EMoveType(READ_MOVE_TYPE(pseudoMoves[i]) == CASTLING)) {
            std::cout << "";
        }
        if (is_move_legal(pseudoMoves[i])) {
            legalMoves.add(pseudoMoves[i], pseudoMoves.get_score(i));
        }
    }
}

Position Position::make_move(std::string algNotation) const
{
    MoveList list;
    generate<LEGAL>(list);

    for (int i = 0; i < list.size(); ++i) {
        if (algNotation == misc::ToString(list[i])) {
            return make_move(list[i]);
        }
    }

    std::cout << "move " << algNotation << " not found\n";
    throw std::invalid_argument("There is no [" + algNotation + "] move in current position");
}

template void Position::generate<PSEUDO>(MoveList&) const;
template void Position::generate<CAPTURES>(MoveList&) const;
template void Position::generate<QUIETS>(MoveList&) const;

