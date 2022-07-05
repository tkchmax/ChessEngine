#pragma once
#include "Board.h"
#include <vector>

enum class EFlag
{
    EMPTY,
    ALPHA,
    EXACT,
    BETA,
};

struct transposition_struct
{
    transposition_struct(U64 zobristHash, EFlag flag, unsigned int depth, int score, std::vector<Move> pv) :
        zobristHash(zobristHash),
        flag(flag),
        //best_move(best_move),
        depth(depth),
        score(score),
        pv(pv){}
    transposition_struct() : transposition_struct(0, EFlag::EMPTY, 0, 0, std::vector<Move>()) {}
    //bool empty() { return best_move.Get() == 0; }
    bool empty() { return flag == EFlag::EMPTY; }

    std::vector<Move> pv;
    U64 zobristHash;
    //Move best_move;
    EFlag flag;
    unsigned int depth;
    int score;
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
    TranspositionTable(const Board& board, size_t table_size_mb = 256);
    ~TranspositionTable();
    void add(transposition_struct new_position);
    void add(const std::vector<Move>& bestMove, EFlag flag, unsigned int depth, int score);
    transposition_struct get();
    void amend_hash(const Move& move);
    void reset_current_hash(const Board& board);

    unsigned int get_size() { return size; }
    U64 get_hash() { return currentHash; }

private:
    ZobristKeys zobristKeys;
    transposition_struct* table;
    U64 capacity;
    unsigned int size;
    U64 currentHash;
};