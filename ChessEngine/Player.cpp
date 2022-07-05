#include "Player.h"
#include "Search.h"
#include <bitset>
#include <iostream>
#include <algorithm>

Move Bot::ChooseMove()
{
    //search_struct ss = search::get_best(*board, color, 6);
    auto ss = search::iterative_deepening(*board, color, 0);
    Move move = ss.pv[0];

    bool expNotation = board->IsExpandedNotationNeeded(move);
    std::cout <<"*"<< move.GetNotation(expNotation) << std::endl;
    return move;
}

std::unique_ptr<Player> Player::Create(EPlayer type, std::shared_ptr<Board> board, EColor color)
{
    switch (type)
    {
    case EPlayer::BOT: return std::make_unique<Bot>(board, color, 7);
    case EPlayer::CONSOLE: return std::make_unique<ConsolePlayer>(board, color);
    }
}

Move ConsolePlayer::ChooseMove()
{
    MoveList availibleMoves = board->GenerateMoveList(color);
    std::string raw_move;

    while (true)
    {
        std::cout << "your move >> ";
        std::cin >> raw_move;
            
        for (auto moveIter = availibleMoves.Get().begin(); moveIter != availibleMoves.Get().end(); ++moveIter) {
            if ((*moveIter).GetNotation(board->IsExpandedNotationNeeded(*moveIter)) == raw_move) {
                return *moveIter;
            }
        }
        std::string err_msg = "ERROR: There is no move " + raw_move + " in current posmoveIterion";
        printf("\033[31m%s\033[0m\n", err_msg.c_str());
        printf("\033[32mAvailible moves:\n%s\033[0m\n", GetAvailibleMoves().c_str());
        
    }

    return Move();
}

std::string ConsolePlayer::GetAvailibleMoves()
{
    MoveList moveList = board->GenerateMoveList(color);
    std::string availible_moves = "";

    for (int figureInt = 0; figureInt <= 6; ++figureInt)
    {
        std::list<Move> figure_list;
        std::copy_if(moveList.Get().begin(), moveList.Get().end(), std::back_inserter(figure_list),
            [figureInt](const Move& move) {
                return move.GetFigure() == figureInt;
            });

        if (!figure_list.empty())
        {
            for (auto move : figure_list)
                availible_moves += move.GetNotation(board->IsExpandedNotationNeeded(move)) + " ";
            availible_moves += "\n";
        }
    }
    return availible_moves;
}
