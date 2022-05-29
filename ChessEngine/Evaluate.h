#pragma once
#include "Board.h"

struct eval_struct
{
    int materialEval;
    int mobilityEval;
    int strategyEval;
    int total;
};

namespace evaluate
{
    eval_struct get_full_eval(
        const Board& board
    );

    int get_eval(
        const Board& board,
        EColor color
    );

    int material_eval(
        const Board& board,
        EColor color
    );

    int strategy_eval(
        const Board& board,
        EColor color
    );

    int mobility_eval(
        const Board& board,
        EColor color
    );

    int optimised_diff_eval(
        const Board& board,
        EFigure figure
    );

    int get_diff_figure_material(
        const Board& board,
        EFigure figure
    );
}