#pragma once
#ifndef CPOSITION_H_
#define CPOSITION_H_
#include <string>
#include "MoveList.h"
#include "Misc.h"
#include "Transposition.h"

class Position
{
public:
    Position() : Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {}
    Position(std::string fen);
    friend std::ostream& operator<<(std::ostream& out, const Position& pos);
    void setFEN(const std::string& fen);

    U64 figures(EFigureType type = EFigureType::ALL_FIGURE_TYPE) const;
    U64 figures(EColor color) const;
    U64 figures(EColor color, EFigureType fig_type) const;
    int figure_count(EFigure fig) const;
    EFigure on_square(int sq) const;
    EFigureType on_square_type(int sq) const;
    ESquare get_en_passant() const;
    EColor side_to_move() const;
    U64 get_zobrist() const;
    int get_castling_rights() const;
    bool is_castling_impeded(CastlingRights cr) const;
    bool is_move_legal(Move move) const;
    bool is_king_attacked(EColor color) const;
    bool is_capture_move(Move move) const;
    bool can_castling(CastlingRights cr) const;
    void set_side2move(EColor color);
    void set_en_passant(ESquare sq);
    U64 attack_to(ESquare square) const;
    EGamePhase phase() const;
    void recalc_zobrist();
    int calculate_phase_score() const;
    EGamePhase calculate_phase() const;
    int phase_score() const;

    Position make_move(Move m) const;
    Position make_move(std::string algNotation) const;

    template<EGenType>
    void generate(MoveList&) const;

private:
    void make_castling(EColor color, ESquare from, ESquare to);
    void move_figure(ESquare from, ESquare to);
    void remove_figure(ESquare from);
    void create_figure(ESquare sq, EFigure fig);

private:
    EFigure board[SQ_NB];
    U64 byTypeBB[FIGURE_TYPE_NB];
    U64 byColorBB[COLOR_NB];

    int castlingRights;
    ESquare enPassant;
    EColor sideToMove;
    EGamePhase gp;
    int gpScore;

    U64 attackedSquares;

    U64 zobrist;

    int figureCount[FIGURE_NB];
};

inline EGamePhase Position::phase() const {
    return gp;
}

inline void Position::recalc_zobrist()
{
    zobrist = Transposition::GetZobristHash(*this);
}

inline int Position::calculate_phase_score() const
{
    using namespace evaluate;
    int score = 0;
    for (int f = KNIGHT; f <= QUEEN; ++f) {
           score +=
            misc::countBits(figures(WHITE, EFigureType(f))) * material_score[OPENNING][f]
            + misc::countBits(figures(BLACK, EFigureType(f))) * material_score[OPENNING][f];
    }
    return score;
}

inline U64 Position::figures(EFigureType type) const {
    return byTypeBB[type];
}

inline U64 Position::figures(EColor c) const {
    return byColorBB[c]; 
}

inline U64 Position::figures(EColor c, EFigureType fig_type) const {
    return byColorBB[c] & byTypeBB[fig_type];
}

inline int Position::figure_count(EFigure fig) const
{
    return figureCount[fig];
}

inline EFigure Position::on_square(int sq) const {
    return board[sq];
}

inline EFigureType Position::on_square_type(int sq) const {
    return type_of(board[sq]);
}

inline ESquare Position::get_en_passant() const {
    return enPassant;
}

inline EColor Position::side_to_move() const
{
    return sideToMove;
}

inline U64 Position::get_zobrist() const
{
    return zobrist;
}

inline int Position::get_castling_rights() const
{
    return castlingRights;
}

inline bool Position::is_castling_impeded(CastlingRights cr) const { 
    return byTypeBB[ALL_FIGURE_TYPE] & castling::pathBlockers[cr];
}

inline bool Position::can_castling(CastlingRights cr) const {
    return castlingRights & cr; 
}

inline void Position::set_side2move(EColor color)
{
    sideToMove = color;
}

inline void Position::set_en_passant(ESquare sq)
{
    enPassant = sq;
}

inline bool Position::is_king_attacked(EColor color) const {
    return attack_to(ESquare(misc::lsb(figures(color, KING)))) & byColorBB[~color];
}

inline bool Position::is_capture_move(Move move) const
{
    return board[READ_TO(move)] != NO_FIGURE;
}

inline void Position::move_figure(ESquare from, ESquare to)
{
    EFigure fig = board[from];
    U64 fromTo = TO_BITBOARD(from) | TO_BITBOARD(to);
    byTypeBB[ALL_FIGURE_TYPE] ^= fromTo;
    byTypeBB[type_of(fig)] ^= fromTo;
    byColorBB[color_of(fig)] ^= fromTo;
    board[from] = NO_FIGURE;
    board[to] = fig;
}

inline void Position::remove_figure(ESquare from)
{
    EFigure fig = board[from];
    U64 fromBB = TO_BITBOARD(from);
    byTypeBB[ALL_FIGURE_TYPE] ^= fromBB;
    byTypeBB[type_of(fig)] ^= fromBB;
    byColorBB[color_of(fig)] ^= fromBB;
    board[from] = NO_FIGURE;
    figureCount[fig]--;

}

inline void Position::create_figure(ESquare sq, EFigure fig)
{
    U64 sqBB = TO_BITBOARD(sq);
    byTypeBB[ALL_FIGURE_TYPE] |= sqBB;
    byTypeBB[type_of(fig)] |= sqBB;
    byColorBB[color_of(fig)] |= sqBB;
    board[sq] = fig;
    figureCount[fig]++;
}

#endif //CPOSITION_H_

