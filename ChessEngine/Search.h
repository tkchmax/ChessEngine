#pragma once
#include "Board.h"
#include <vector>
#include <iostream>
#include <iomanip>

struct search_struct
{
    void show() {

        std::cout << "score: " << std::setprecision(3) << (double)score / 100 << " nodes: " << nNodes << " ";
        for (const auto& m : pv) {
            std::cout << m.GetNotation() << " ";
        }
        std::cout << std::endl;
        //for (const auto& m : pv) {
        //    std::cout << m.Get() << " ";
        //}
    }

    std::vector<Move> pv;
    int score;
    int nNodes;
};

namespace search
{
    search_struct get_best(
        Board board,
        EColor color,
        int depth
    );

    search_struct iterative_deepening(
        Board board,
        EColor color,
        int time
    );

}
