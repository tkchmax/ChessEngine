#include <iostream>
#include "Game.h"

#include "Evaluate.h"
#include "Search.h"
#include "Transposition.h"
#include <chrono>

using namespace std;

int main()
{
    using namespace misc;
    Game game(EPlayer::BOT, EPlayer::CONSOLE);
    //Game game(EPlayer::CONSOLE, EPlayer::BOT);
    //game.play();

    Board b;
    b.SetFEN("r1b1kb1r/2pp1ppp/1np1q3/p3P3/2P5/1P6/PB1NQPPP/R3KB1R w KQkq - 0 1");
    //b.SetFEN("8/p4kpp/2p1p3/8/6K1/2q5/8/7r w - -");

    //auto start = chrono::high_resolution_clock::now();
    ////auto ss = search::get_best(b, WHITE, 6);
    //auto end = chrono::high_resolution_clock::now();
    //cout << "time1: " << chrono::duration<float>(end - start).count() << endl << endl;

    auto start = chrono::high_resolution_clock::now();
    search::iterative_deepening(b, WHITE, 0);
    auto end = chrono::high_resolution_clock::now();
    cout << "time2: " << chrono::duration<float>(end - start).count() << std::endl;
}

