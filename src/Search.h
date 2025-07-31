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
        s_search() {}
        s_search(const std::vector<Move>& pv, long long time, U64 nodes, int score) : 
            pv(pv),
            time(time),
            nNodes(nodes),
            score(score)
        {}
        std::vector<Move> pv;
        long long time;
        U64 nNodes;
        int score;
    };

    s_search search(int depth, const Position& pos);
    s_search iterative_deepening(int depth, const Position& pos);

    struct PerftStat {
        friend std::ostream& operator<<(std::ostream& out, const PerftStat& s);
        PerftStat() :
            nCapture(0),
            nCastling(0),
            nEnpassant(0),
            nPromotion(0),
            nCapturePerFigure()
        {}
        int nCapture;
        int nCastling;
        int nEnpassant;
        int nPromotion;
        std::array<int,FIGURE_NB> nCapturePerFigure;
    };

    U64 perft(const Position& pos, int depth, PerftStat* stat = nullptr);
}

#endif // SEARCH_H_

