#pragma once
#ifndef SEARCH_H_
#define SEARCH_H_
#include "Constants.h"
#include <vector>
#include <iostream>

class Position;
namespace search {
    struct s_search {
        friend std::ostream& operator<<(std::ostream& out, const s_search& s);
        std::vector<Move> pv;
        long long time;
        U64 nNodes;
        int score;
    };

    s_search search(int depth, const Position& pos);
    s_search iterative_deepening(int depth, const Position& pos);

    U64 perft(const Position& pos, int depth);
}

#endif // SEARCH_H_

