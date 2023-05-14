#pragma once
#ifndef MOVELIST_H_
#define MOVELIST_H_
#include "Constants.h"
#include <iostream>
#include <vector>

using KillerHeuristic = std::array<std::array<Move, MAX_PLY>, 2>;
using HistoryHeuristic = std::array<std::array<std::array<int, SQ_NB>, SQ_NB>, COLOR_NB>;
using CounterMoveHeuristic = std::array<std::array<Move, SQ_NB>, SQ_NB>;

std::string GetAlgebraicNotation(Move move);

enum EMoveType {
    NORMAL,
    DOUBLE_PUSH,
    PROMOTION,
    EN_PASSANT,
    CASTLING
};

template<EMoveType t>
constexpr U32 EncodeMove(ESquare from, ESquare to, EFigureType figure_type) {
    return U32(from | (to << 6) | (figure_type << 12) | (t << 15));
}

struct s_move {
    U32 move;
    int score;
};

class MoveList
{
public:
    friend std::ostream& operator<<(std::ostream& out, const MoveList& list);
    Move operator[](int idx) const;
    MoveList() : len(0) {}
    MoveList(const std::vector<Move>& moves);
    int get_score(int idx) const;
    void add(const U32& move, int score = 0);
    void pick(int idx);
    int size() const { return len; }
    void sort(const KillerHeuristic&,
              const HistoryHeuristic&,
              const CounterMoveHeuristic&,
              Move cacheMove,
              Move prevMove,
              Move pvMove,
              int,
              EColor);

private:
    s_move list[cmove_list::capacity];
    int len;
};

#endif //CMOVELIST_H_

