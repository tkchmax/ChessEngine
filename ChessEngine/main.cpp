#include <iostream>
#include "Rays.h"
#include "Misc.h"
#include "Figure.h"

using namespace std;

int main()
{
    Figure* fig = new Rook(WHITE, A1);
    misc::ShowBits(fig->GetMoves(0, 0));
    Rook* r = static_cast<Rook*>(fig);
    misc::ShowBits(r->GetMoves(1ULL<<5, 0));
    misc::ShowBits(1ULL << 5);

}

