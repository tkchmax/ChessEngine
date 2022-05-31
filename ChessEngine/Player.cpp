#include "Player.h"
#include "Search.h"
#include <bitset>
#include <iostream>
#include <algorithm>

namespace
{
    std::array<int, 64> GetPriority(EFigure figure, EColor color, EGamePhase gamePhase)
    {
        switch (figure)
        {
        case EFigure::KNIGHT: return figure_priority_squares::KNIGHT;
        case EFigure::BISHOP: return figure_priority_squares::BISHOP;
        case EFigure::ROOK: return figure_priority_squares::ROOK;
        case EFigure::QUEEN: return figure_priority_squares::QUEEN;

        case EFigure::PAWN: return (color == WHITE) ? figure_priority_squares::WHITE_PAWN : figure_priority_squares::BLACK_PAWN;
        case EFigure::KING:
        {
            if (gamePhase == EGamePhase::END_GAME) {
                return figure_priority_squares::KING_ENDGAME;
            }
            else {
                return (color == WHITE) ? figure_priority_squares::WHITE_KING_MIDDLEGAME : figure_priority_squares::BLACK_KING_MIDDLEGAME;
            }
        }
        }
    }

    std::array<std::array<std::array<std::array<int, 64>, 3>, 6>, 2> GeneratePrioritiesArray()
    {
        std::array<std::array<std::array<std::array<int, 64>, 3>, 6>, 2> priorities{};
        for (unsigned colorInt = 0; colorInt < 2; ++colorInt) {
            for (unsigned figureInt = 0; figureInt < 6; ++figureInt) {
                for (unsigned gamePhaseInt = 0; gamePhaseInt < 3; ++gamePhaseInt) {
                    priorities[colorInt][figureInt][gamePhaseInt] = GetPriority(static_cast<EFigure>(figureInt),
                        static_cast<EColor>(colorInt), static_cast<EGamePhase>(gamePhaseInt));
                }
            }
        }
        return priorities;
    }
}

Move Bot::ChooseMove()
{
    search_struct ss = search::get_best(*board, color, depth);
    Move move = ss.bestMove;
    std::cout << move.GetNotation(board->IsExpandedNotationNeeded(move)) << std::endl;
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

    for (int figureInt = 0; figureInt <= EFigure::COUNT; ++figureInt)
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
