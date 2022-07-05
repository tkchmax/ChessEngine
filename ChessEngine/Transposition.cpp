#include "Transposition.h"
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <set>

namespace {
    TranspositionTable::ZobristKeys init_zobrist() {
        TranspositionTable::ZobristKeys keys;
        for (int colorInt = 0; colorInt < 2; ++colorInt) {
            for (int figureInt = 0; figureInt < 6; ++figureInt) {
                for (int squareInt = 0; squareInt < 64; ++squareInt) {
                    keys[colorInt][figureInt][squareInt] = misc::rand64();
                }
            }
        }
        std::vector<U64> t;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 6; ++j) {
                for (int k = 0; k < 64; ++k) {
                    t.push_back(keys[i][j][k]);
                }
            }
        }

        std::set<U64> set(t.begin(), t.end());

        if (t.size() != set.size()) {
            std::cout << "not unqiue zobrist keys\n";
        }

        return keys;
    }
}

TranspositionTable::TranspositionTable(const Board& board, size_t table_capacity_mb)
{
    size = 0;
    capacity = (size_t)(table_capacity_mb * (1024 * 1024)) / sizeof(transposition_struct);

    //making binary represent of a capacity as 00...00111 <=> 2^n-1
    while ((capacity & (capacity - 1)) != 0) {
        capacity -= 1;
    }
    capacity -= 1;

    table = (transposition_struct*)calloc(capacity + 1, sizeof(transposition_struct));

    zobristKeys = init_zobrist();
    reset_current_hash(board);
}

TranspositionTable::~TranspositionTable()
{
    if (table != nullptr) {
        free(table);
    }
}

void TranspositionTable::add(transposition_struct new_position)
{
    int id = currentHash & capacity;
    assert(id < capacity);

    if (table[id].empty()) {
        table[id] = new_position;
        size += 1;
    }
    else if (new_position.depth >= table[id].depth && new_position.zobristHash == table[id].zobristHash) {
        table[id] = new_position;
    }
}

void TranspositionTable::add(const std::vector<Move>& pv, EFlag flag, unsigned int depth, int score)
{
    add(transposition_struct(currentHash, flag, depth, score, pv));
}


transposition_struct TranspositionTable::get()
{
    auto ss = table[currentHash & capacity];
    if (!ss.empty() && ss.zobristHash == currentHash) {
        return ss;
    }
    return transposition_struct();
}

void TranspositionTable::amend_hash(const Move& move)
{
    EColor color = move.GetMoveColor();
    EFigure figure = move.GetFigure();
    EFigure capture = move.GetCapture();
    ESquare from = move.GetFrom();
    ESquare to = move.GetTo();

    //delete capture figure position
    if (capture != EFigure::NO_FIGURE) {
        currentHash ^= zobristKeys[(color == WHITE) ? BLACK : WHITE][capture][to];
    }

    //delete old position
    currentHash ^= zobristKeys[color][figure][from];

    //set final position
    currentHash ^= zobristKeys[color][figure][to];

    using namespace positions_square;
    switch (move.GetMoveType()) {
    case EMoveType::SHORT_CASTLING:
        currentHash ^= zobristKeys[color][ROOK][(color == WHITE) ? WHITE_RSH_ROOK : BLACK_RSH_ROOK];
        currentHash ^= zobristKeys[color][ROOK][(color == WHITE) ? WHITE_RSH_ROOK_CASTLED : BLACK_RSH_ROOK_CASTLED];
        break;
    case EMoveType::LONG_CASTLING:
        currentHash ^= zobristKeys[color][ROOK][(color == WHITE) ? WHITE_LSH_ROOK : BLACK_LSH_ROOK];
        currentHash ^= zobristKeys[color][ROOK][(color == WHITE) ? WHITE_LSH_ROOK_CASTLED : BLACK_LSH_ROOK_CASTLED];
        break;
    case EMoveType::PAWN_TO_KNIGHT:
        assert(figure == PAWN);
        currentHash ^= zobristKeys[color][PAWN][to];
        currentHash ^= zobristKeys[color][KNIGHT][to];
        break;
    case EMoveType::PAWN_TO_BISHOP:
        currentHash ^= zobristKeys[color][PAWN][to];
        currentHash ^= zobristKeys[color][BISHOP][to];
        break;
    case EMoveType::PAWN_TO_ROOK:
        currentHash ^= zobristKeys[color][PAWN][to];
        currentHash ^= zobristKeys[color][ROOK][to];
        break;
    case EMoveType::PAWN_TO_QUEEN:
        assert(figure == PAWN);
        currentHash ^= zobristKeys[color][PAWN][to];
        currentHash ^= zobristKeys[color][QUEEN][to];
        break;
    }
}

void TranspositionTable::reset_current_hash(const Board& board)
{
    currentHash = (U64)0xF3218F1C9510786C;
    const auto& figures = board.GetFigureFromCoord();
    for (int colorInt = 0; colorInt < 2; ++colorInt) {
        for (int squareInt = 0; squareInt < 64; ++squareInt) {
            int figureInt = figures[colorInt][squareInt];
            if (figureInt != EFigure::NO_FIGURE) {
                currentHash ^= zobristKeys[colorInt][figureInt][squareInt];
            }
        }
    }
}
