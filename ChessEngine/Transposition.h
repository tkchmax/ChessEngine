#pragma once
#include "Board.h"

struct transposition_struct
{
    transposition_struct() : best_move(Move(0)), alpha(0) {}
    transposition_struct(Move m, int alpha) : best_move(m), alpha(alpha) {}
    bool empty() { return best_move.Get() == 0; }

    Move best_move;
    int alpha;
};

class TranspositionTable
{
public:
    using ZobristKeys = std::array<std::array<std::array<U64, 64>, 6>, 2>;
    TranspositionTable() :
        zobristKeys(ZobristKeys{}),
        table(nullptr),
        capacity(0),
        size(0),
        currentHash(0) {};
    TranspositionTable(const Board& board, size_t table_size_mb = 128);
    ~TranspositionTable();
    void add(transposition_struct new_position);
    transposition_struct get();
    void amend_hash(const Move& move);
    void reset_current_hash(const Board& board);

    unsigned int get_size() { return size; }

private:
    ZobristKeys zobristKeys;
    transposition_struct* table;
    U64 capacity;
    unsigned int size;
    U64 currentHash;
};