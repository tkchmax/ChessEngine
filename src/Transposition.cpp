#include "Transposition.h"
#include "Misc.h"
#include "Position.h"
#include <vector>
#include <set>
#include <cassert>

namespace {

    Transposition::ZobristKeys GeneratePieceKeys() {
        srand(time(0));

        Transposition::ZobristKeys keys;
        for (int i = 0; i < 1000; ++i) {
            for (int c = 0; c < COLOR_NB; ++c) {
                for (int f = PAWN; f < FIGURE_TYPE_NB; ++f) {
                    for (int sq = 0; sq < SQ_NB; ++sq) {
                        keys[c][f][sq] = misc::rand64();
                    }
                }
            }

            std::vector<U64> t;
            for (int i = 0; i < COLOR_NB; ++i) {
                for (int j = PAWN; j < FIGURE_TYPE_NB; ++j) {
                    for (int k = 0; k < SQ_NB; ++k) {
                        t.push_back(keys[i][j][k]);
                    }
                }
            }

            std::set<U64> uniqueSet(t.begin(), t.end());
            if (t.size() == uniqueSet.size()) {
                return keys;
            }


        }
        assert(false);
        return keys;
    }

    std::array<U64, SQ_NB> GenerateEnPassantKeys() {
        std::array<U64, SQ_NB> keys{};
        for (int i = 0; i < SQ_NB; ++i) {
            keys[i] = misc::rand64();
        }
        return keys;
    }

    std::array<U64, CR_NB> GenerateCastlingKeys() {
        std::array<U64, CR_NB> keys{};
        for (int i = 0; i < CR_NB; ++i) {
            keys[i] = misc::rand64();
        }
        return keys;
    }
}

const s_node* const s_node::EMPTY = new s_node();
Transposition::ZobristKeys Transposition::zobristKeys = GeneratePieceKeys();
std::array<U64, SQ_NB> Transposition::enPassantKeys = GenerateEnPassantKeys();
std::array<U64, CR_NB> Transposition::castlingKeys = GenerateCastlingKeys();
U64 Transposition::sideKey = misc::rand64();


U64 Transposition::GetZobristHash(const Position& pos)
{
    U64 hash = (U64)0xF3218F1C9510786C;

    for (int sq = 0; sq < 64; ++sq) {
        EFigure fig = pos.on_square(sq);
        if (fig != NO_FIGURE) {
            hash ^= zobristKeys[color_of(fig)][type_of(fig)][sq];
        }
    }

    if (pos.get_en_passant() != SQ_NONE) {
        hash ^= enPassantKeys[pos.get_en_passant()];
    }

    //if (pos.can_castling(WHITE_00)) {
    //    hash ^= castlingKeys[WHITE_00];
    //}
    //if (pos.can_castling(BLACK_00)) {
    //    hash ^= castlingKeys[BLACK_00];
    //}
    //if (pos.can_castling(WHITE_000)) {
    //    hash ^= castlingKeys[WHITE_000];
    //}
    //if (pos.can_castling(BLACK_000)) {
    //    hash ^= castlingKeys[BLACK_000];
    //}

    if (pos.side_to_move() == BLACK) {
        hash ^= sideKey;
    }

    return hash;
}

Transposition::Transposition(unsigned int table_capacity_mb)
{
    size = 0;
    capacity = (table_capacity_mb * 1024 * 1024) / sizeof(s_node);

    //making capacity equal 2^x
    while ((capacity & (capacity - 1)) != 0) {
        capacity -= 1;
    }
    capacity -= 1;

    table = (s_node*)calloc(capacity + 1, sizeof(s_node));
    for (int i = 0; i < capacity; ++i) {
        table[i].flag = HASH_EMPTY;
        table[i].score = 0;
        table[i].bestMove = 0;
        table[i].depth = 0;
    }
}

Transposition::~Transposition()
{
    if (table != nullptr) {
        free(table);
    }
}

void Transposition::add(U64 hash, Move bestMove, int depth, int score, EHashFlag flag)
{
    int id = hash % capacity;
    s_node newNode(hash, bestMove, depth, score, flag);

    //if (table[id].empty()) {
    //    table[id] = newNode;
    //    size += 1;
    //}
    //else if (hash == table[id].zobrist && depth >= table[id].depth) {
    //    table[id] = newNode;
    //}

    if (table[id].empty()) {
        size += 1;
    }
    table[id] = newNode;
}

void Transposition::clean()
{
    for (int i = 0; i < capacity; ++i) {
        table[i].flag = HASH_EMPTY;
        table[i].score = 0;
        table[i].bestMove = 0;
        table[i].depth = 0;
    }
}


void Transposition::AmmendHash(U64& hash, Move move, const Position& pos)
{
    ESquare from = ESquare(READ_FROM(move));
    ESquare to = ESquare(READ_TO(move));
    EFigureType fig = EFigureType(READ_FIGURE(move));
    EMoveType mt = EMoveType(READ_MOVE_TYPE(move));

    EFigure capture = mt == CASTLING ? NO_FIGURE : pos.on_square(to);
    EColor color = pos.side_to_move();

    if (capture != NO_FIGURE) {
        //delete capture sq
        hash ^= zobristKeys[~color][type_of(capture)][to];
    }

    //delete old position
    hash ^= mt == PROMOTION ? zobristKeys[color][PAWN][from] : zobristKeys[color][fig][from];

    if (mt == CASTLING) {
        assert(capture == NO_FIGURE);

        ESquare rf = castling::rookFromTo00[color][0];
        ESquare rto = castling::rookFromTo00[color][1];
        ESquare kto = castling::kingTo00[color];

        //if long castling
        if (from > to) {
            rf = castling::rookFromTo000[color][0];
            rto = castling::rookFromTo000[color][1];
            kto = castling::kingTo000[color];
        }

        hash ^= zobristKeys[color][KING][kto];
        hash ^= zobristKeys[color][ROOK][rf];
        hash ^= zobristKeys[color][ROOK][rto];
    }

    else if (mt == EN_PASSANT) {
        assert(capture == NO_FIGURE);

        //delete capture
        hash ^= zobristKeys[~color][PAWN][color == WHITE ? to - 8 : to + 8];
        //set new pawn square
        hash ^= zobristKeys[color][PAWN][to];
    }
    else if (mt == PROMOTION) {
        //set new figure
        hash ^= zobristKeys[color][fig][to];
    }
    else {  //NORMAL and DOUBLE_PUSH
        if (mt == DOUBLE_PUSH) {
            hash ^= enPassantKeys[to];
        }

        //set new square
        hash ^= zobristKeys[color][fig][to];
    }

    if (pos.get_en_passant() != SQ_NONE) {
        hash ^= enPassantKeys[pos.get_en_passant()];
    }

    hash ^= sideKey;
}

const s_node* const Transposition::get(U64 hash) const
{
    const s_node* node = &table[hash % capacity];
    return node->zobrist == hash ? node : s_node::EMPTY;
}


