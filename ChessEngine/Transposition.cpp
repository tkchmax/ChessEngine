#include "Transposition.h"
#include <iostream>
#include <cstdlib>
#include <cassert>

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
        size += 1;
    }
    table[id] = new_position;
}

transposition_struct TranspositionTable::get()
{
    return table[currentHash & capacity];
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
