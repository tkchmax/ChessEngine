#include "UCI.h"
#include "Position.h"
#include "Search.h"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <time.h>

extern Position uciPosition;
extern U64 repetitionTable[1000];
extern int repetitionIndex;

std::unordered_map<std::string, UCI*> UCI::uci = std::unordered_map<std::string, UCI*>();
UCI::Flags UCI::flags = UCI::Flags();

std::vector<std::string> UCI_ISREADY::handle(std::vector<std::string> args)
{
    return { "readyok" };
    //return {};
}

std::vector<std::string> UCI_UCI::handle(std::vector<std::string> args)
{
    return { "id name Karasb", "id author tkchmax", "uciok" };
    //return {};

}

std::vector<std::string> UCI_POSITION::handle(std::vector<std::string> args)
{
    int movesIdx = 0;
    if (args.at(1) == "fen") {
        uciPosition.setFEN(args.at(2));
        movesIdx = 3;
    }

    else if (args.at(1) == "startpos") {
        uciPosition.setFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        movesIdx = 2;
    }

    if (movesIdx && args.at(movesIdx) == "moves") {
        repetitionIndex = 0;
        for (int i = movesIdx + 1; i < args.size(); ++i) {
            uciPosition = uciPosition.make_move(args[i]);
            if(i != args.size() - 1 )
                repetitionTable[repetitionIndex++] = uciPosition.get_zobrist();
        }
    }
    
    return {};
}

std::vector<std::string> UCI_DRAW::handle(std::vector<std::string> args)
{
    std::cout << uciPosition << std::endl;
    return {};
}

std::vector<std::string> UCI_GO::handle(std::vector<std::string> args)
{
    //default depth
    int depth = 64;

    auto argument = args.begin();
    try {
        if ((argument = std::find(args.begin(), args.end(), "depth")) != args.end()) {
            depth = std::stoi(*++argument);
        }

        if ((argument = std::find(args.begin(), args.end(), "movetime")) != args.end())
        {
            UCI::flags.time = std::stoi(*++argument);
        }
    }catch(...){}

    auto t = std::chrono::high_resolution_clock::now();
    
    UCI::flags.starttime = UCI::get_time_ms();

    if (UCI::flags.time != 0) {
        UCI::flags.timeset = true;
        UCI::flags.time -= 50;
        UCI::flags.stoptime = UCI::flags.starttime + UCI::flags.time;
    }

    std::cout <<
        " time: " << UCI::flags.time <<
        " start: " << UCI::flags.starttime <<
        " stop: " << UCI::flags.stoptime <<
        " depth: " << depth << std::endl;

    search::s_search res = search::iterative_deepening(depth, uciPosition);

    return { "bestmove " + misc::ToString(res.pv.at(0)) };
}

void UCI::init()
{
    uci["isready"] = new UCI_ISREADY();
    uci["uci"] = new UCI_UCI();
    uci["position"] = new UCI_POSITION();
    uci["d"] = new UCI_DRAW();
    uci["go"] = new UCI_GO();

    for (const auto& s : uci["uci"]->handle({})) {
        std::cout << s << std::endl;
    }
}

std::vector<std::string> UCI::parse(std::string cmd)
{
    std::vector<std::string> tokens;
    std::stringstream stream(cmd);
    std::string token;
    bool in_quotes = false;
    std::string current_token;
    char c;
    while (stream.get(c)) {
        if (c == '"') {
            in_quotes = !in_quotes;
        }
        else if (isspace(c) && !in_quotes) {
            tokens.push_back(current_token);
            current_token.clear();
        }
        else {
            current_token += c;
        }
    }
    if (!current_token.empty()) {
        tokens.push_back(current_token);
    }
    return tokens;
}

void UCI::loop()
{
    std::string Line;
    std::cout.setf(std::ios::unitbuf);
    while (std::getline(std::cin, Line)) {
        auto args = UCI::parse(Line);
        try {
            auto out = uci.at(args.at(0))->handle(args);
            for (const auto& str : out) {
                std::cout << str << std::endl;
            }
        }
        catch (...) {}
    }
}

long long UCI::get_time_ms()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
           ).count();
}
