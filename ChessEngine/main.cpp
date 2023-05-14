#include <iostream>
#include "Game.h"
#include "Search.h"


int main()
{
    Position pos;
    std::cout << search::iterative_deepening(14, pos);
}


