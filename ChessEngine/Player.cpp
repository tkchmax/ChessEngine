#include "Player.h"

Move Bot::ChooseMove()
{
    bestMoves = std::vector<Move>(depth);
    int score = AlphaBeta(depth, -INF, INF, color);
    return bestMoves[depth - 1];
}

int Bot::AlphaBeta(int depth, int alpha, int beta, EColor color)
{
    if (depth == 0) {
        return Quies(color, alpha, beta);
    }

    MoveList moves = board->GenerateMoveList(color);
    //mvv_lva

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
    //mvv-lva

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
    return 0;
}

int Bot::GetMaterialEval(EColor color) const
{
    int eval = 0;
    return eval;
}
