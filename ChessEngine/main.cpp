#include <iostream>
#include "Game.h"
#include "Search.h"
#include <chrono>
#include "UCI.h"
#include "Evaluate.h"
Position uciPosition;

int main()
{
    UCI::init();
    UCI::loop();
}


