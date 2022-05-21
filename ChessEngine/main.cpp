#include <iostream>
#include "Rays.h"
#include "Misc.h"
#include "Figure.h"

using namespace std;

int main()
{

    U64 op = TO_BITBOARD(C1) | TO_BITBOARD(E1);
    Pawn p(BLACK, D2);
    //misc::ShowBits(TO_BITBOARD(5));
    misc::ShowBits(p.GetMoves(0, op));
}

