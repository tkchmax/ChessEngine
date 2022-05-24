#include "Player.h"
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
    bestMoves = std::vector<Move>(depth);
    int score = AlphaBeta(depth, -INF, INF, color);
    Move move = bestMoves[depth - 1];
    std::cout << move.GetNotation(board->IsExpandedNotationNeeded(move)) << std::endl;
    return move;
}

int Bot::AlphaBeta(int depth, int alpha, int beta, EColor color)
{
    if (depth == 0) {
        return Quies(color, alpha, beta);
    }

    MoveList moves = board->GenerateMoveList(color);
    moves.mvv_lva();

    auto killerMove = std::find(moves.Get().begin(), moves.Get().end(), bestMoves[depth - 1]);
    if (killerMove != moves.Get().end()) {
        moves.replaceToPriorities(killerMove);
    }

    auto moveIter = moves.Get().begin();
    Move bestMove = *moveIter;

    while (moveIter != moves.Get().end() && alpha < beta) {
        board->makeMove(*moveIter);
        int temp = INF;
        if (!board->IsGameOver()) {
            temp = -AlphaBeta(depth - 1, -beta, -alpha, (color == WHITE) ? BLACK : WHITE);
        }
        board->undoMove();

        if (temp > alpha) {
            alpha = temp;
            bestMove = *moveIter;
            bestMoves[depth - 1] = bestMove;
        }
        moveIter++;
    }

    return alpha;
}

int Bot::Quies(EColor color, int alpha, int beta)
{
    int eval = Evaluate(color);
    if (eval > alpha) {
        alpha = eval;
    }

    MoveList forcedMoves = board->GenerateMoveList(color, true);
    forcedMoves.mvv_lva();

    auto moveIter = forcedMoves.Get().begin();
    while (moveIter != forcedMoves.Get().end() && alpha < beta) {
        board->makeMove(*moveIter);
        eval = -Quies((color == WHITE) ? BLACK : WHITE, -beta, -alpha);
        board->undoMove();

        if (eval > alpha) {
            alpha = eval;
        }
        moveIter++;
    }
    return alpha;
}

int Bot::Evaluate(EColor color) const
{
    int eval = 0;
    for (int figureInt = 0; figureInt < EFigure::COUNT; ++figureInt) {
        EFigure figure = static_cast<EFigure>(figureInt);
        eval += GetMaterialEval(figure) + GetStrategyEval(figure);
    }
    return (color == WHITE) ? eval : -eval;
}

int Bot::GetMaterialEval(EFigure figure) const
{
    static const std::array<int, EFigure::COUNT> costs = {
        cost_default::PAWN,
        cost_default::KNIGHT,
        cost_default::BISHOP,
        cost_default::ROOK,
        cost_default::QUEEN,
        cost_default::KING
    };

    return costs[figure] * (board->GetFigureCount(WHITE, figure) - board->GetFigureCount(BLACK, figure));
}

int Bot::GetStrategyEval(EFigure figure) const
{
    static const auto priorities = GeneratePrioritiesArray();
    static const double mobilityCoef = 0.5;

    int eval = 0;
    const auto& figures = board->GetFigures();
    EGamePhase gamePhase = board->GetGamePhase();

    U64 whiteBoard = board->GetSideBoard(WHITE);
    U64 blackBoard = board->GetSideBoard(BLACK);

    const auto& whiteList = figures[WHITE][figure];
    for (auto figureIter = whiteList.begin(); figureIter != whiteList.end(); figureIter++) {
        eval += priorities[WHITE][figure][gamePhase][(*figureIter)->GetSquare()]; //position eval
        U64 silentMoves = (*figureIter)->GetSilentMoves(whiteBoard, blackBoard);
        eval += mobilityCoef * std::bitset<64>(silentMoves).count(); // mobility eval
    }

    const auto& blackList = figures[BLACK][figure];
    for (auto figureIter = blackList.begin(); figureIter != blackList.end(); ++figureIter) {
        eval -= priorities[BLACK][figure][gamePhase][(*figureIter)->GetSquare()];
        U64 silentMoves = (*figureIter)->GetSilentMoves(blackBoard, whiteBoard);
        eval -= mobilityCoef * std::bitset<64>(silentMoves).count();
    }

    return eval;

}

std::unique_ptr<Player> Player::Create(EPlayer type, std::shared_ptr<Board> board, EColor color)
{
    switch (type)
    {
    case EPlayer::BOT: return std::make_unique<Bot>(board, color, 6);
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
