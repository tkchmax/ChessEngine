#include "Player.h"
#include "Position.h"
#include "Game.h"
#include "Search.h"
#include "Misc.h"
#include <vector>

namespace {
    std::string GetAvailibleMoves(const Position& pos) {
        MoveList moves;
        pos.generate<LEGAL>(moves);
        std::string availibleMoves = "";

        for (int f = PAWN; f <= KING; ++f) {
            bool newline = false;
            for (int i = 0; i < moves.size(); ++i) {
                if (READ_FIGURE(moves[i]) == f) {
                    newline = true;
                    availibleMoves += misc::ToString(moves[i]) + " ";
                }
            }
            if(newline)
                availibleMoves += "\n";
        }

        return availibleMoves;
    }
}

std::unique_ptr<Player> Player::Create(EColor color, EPlayer type, const std::vector<Position>* gameHist)
{
    switch (type)
    {
    case EPlayer::BOT: return std::make_unique<Bot>(color, gameHist, 13);
    case EPlayer::CONSOLE: return std::make_unique<ConsolePlayer>(color, gameHist);
    }
}

Move ConsolePlayer::pick_move()
{
    //get current position
    const Position& pos = gameHist->back();

    MoveList moves;
    pos.generate<LEGAL>(moves);

    std::string in_move;
    while (true) {
        std::cout << "your move >> ";
        std::cin >> in_move;

        for (int i = 0; i < moves.size(); ++i) {
            if (misc::ToString(moves[i]) == in_move) {
                return moves[i];
            }
        }

        std::string err_msg = "There is no move [" + in_move + "] in the current position";
        printf("\033[31m%s\033[0m\n", err_msg.c_str());
        printf("\033[32mAvailible moves:\n%s\033[0m\n", GetAvailibleMoves(pos).c_str());

    }
}

Move Bot::pick_move()
{
    const Position& pos = gameHist->back();
    search::s_search res = search::iterative_deepening(depth, pos);
    return res.pv[0];
}
