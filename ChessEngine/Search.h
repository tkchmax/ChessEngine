#pragma once
#include "Board.h"

struct search_struct
{
    Move bestMove;
};

namespace search
{
    search_struct get_best(
        Board board,
        EColor color,
        int depth
    );

}
