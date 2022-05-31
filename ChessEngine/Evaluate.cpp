#include "Evaluate.h"
#include <bitset>
#include <iostream>

namespace
{
    constexpr std::array<int, EFigure::COUNT> figure_costs = {
        cost_default::PAWN,
        cost_default::KNIGHT,
        cost_default::BISHOP,
        cost_default::ROOK,
        cost_default::QUEEN,
        cost_default::KING
    };

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

eval_struct evaluate::get_full_eval(const Board& board)
{
    return eval_struct();
}

int evaluate::get_eval(const Board& board, EColor color)
{
    int eval = 0;
    for (int figureInt = 0; figureInt < EFigure::COUNT; ++figureInt) {
        EFigure figure = static_cast<EFigure>(figureInt);
        eval += get_diff_figure_material(board, figure) + optimised_diff_eval(board, figure);
    }
    return (color == WHITE) ? eval : -eval;
}

int evaluate::material_eval(const Board& board, EColor color)
{
    int eval = 0;
    for (int figureInt = 0; figureInt < 6; ++figureInt) {
        EFigure figure = static_cast<EFigure>(figureInt);
        eval += figure_costs[figureInt] * (board.GetFigureCount(WHITE, figure) - board.GetFigureCount(BLACK, figure));
    }
    return (color == WHITE) ? eval : -eval;

}

int evaluate::optimised_diff_eval(const Board& board, EFigure figure)
{
    static const auto priorities = GeneratePrioritiesArray();
    static const double mobilityCoef = 0.5;

    int eval = 0;
    const auto& figures = board.GetFigures();
    EGamePhase gamePhase = board.GetGamePhase();

    U64 whiteBoard = board.GetSideBoard(WHITE);
    U64 blackBoard = board.GetSideBoard(BLACK);

    auto eval_each_figure = [&](const std::list<std::unique_ptr<Figure>>& list, EColor color, U64 blockers, U64 opposite) {
        int eval = 0;
        for (auto figureIter = list.begin(); figureIter != list.end(); figureIter++) {
            //strategy eval
            eval += priorities[color][figure][gamePhase][(*figureIter)->GetSquare()];

            //mobility eval
            U64 silentMoves = (*figureIter)->GetSilentMoves(blockers, opposite);
            eval += mobilityCoef * std::bitset<64>(silentMoves).count();
        }
        return eval;
    };

    const auto& whiteList = figures[WHITE][figure];
    eval += eval_each_figure(whiteList, WHITE, whiteBoard, blackBoard);

    const auto& blackList = figures[BLACK][figure];
    eval -= eval_each_figure(blackList, BLACK, blackBoard, whiteBoard);

    return eval;
}

int evaluate::get_diff_figure_material(const Board& board, EFigure figure)
{
    return figure_costs[figure] * (board.GetFigureCount(WHITE, figure) - board.GetFigureCount(BLACK, figure));
}
