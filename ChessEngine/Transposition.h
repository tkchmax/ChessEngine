#pragma once
#ifndef TRANSPOSITION_H_
#define TRANSPOSITION_H_
#include "Constants.h"

enum EHashFlag {
    HASH_EMPTY,
    HASH_ALPHA,
    HASH_EXACT,
    HASH_BETA
};

struct s_node {
    s_node() : zobrist(0), bestMove(0), depth(0), score(0), flag(HASH_EMPTY) {}
    s_node(U64 zobrist, Move move, int depth, int score, EHashFlag flag) :
        zobrist(zobrist),
        bestMove(move),
        depth(depth),
        score(score),
        flag(flag) {}
    bool empty() { return flag == HASH_EMPTY; }
    static const s_node* const EMPTY;

    U64 zobrist;
    Move bestMove;
    int depth;
    int score;
    EHashFlag flag;
};

class Position;
class Transposition
{
public:
    using ZobristKeys = std::array<std::array<std::array<U64, SQ_NB>, FIGURE_TYPE_NB>, COLOR_NB>;
    static U64 GetZobristHash(const Position& pos);
    static void AmmendHash(U64& hash, Move move, const Position& pos);

    Transposition(unsigned int table_capacity_mb);
    Transposition() : Transposition(256) {}
    ~Transposition();

    void add(U64 hash, Move bestMove, int depth, int score, EHashFlag flag);
    const s_node* const get(U64 hash) const;
    const int get_capacity() const { return capacity; }
//private:
public:
    static ZobristKeys zobristKeys;
    static std::array<U64, SQ_NB> enPassantKeys;
    static std::array<U64, CR_NB> castlingKeys;
    static U64 sideKey;
    s_node* table;
    size_t size;
    size_t capacity;
};

#endif //TRANSPOSITION_H_

