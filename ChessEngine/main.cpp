#include <iostream>
#include "Game.h"
#include "Search.h"
#include <chrono>
#include "UCI.h"
#include "Evaluate.h"
#include "Rays.h"
Position uciPosition;

int main()
{
    UCI::init();
    UCI::loop();
}


