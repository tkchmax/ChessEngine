#include "Search.h"
#include "Evaluate.h"
#include <vector>

namespace
{
    static std::vector<Move> bestMoves;

    int quies(EColor color, int alpha, int beta, Board& board)
    {
        int eval = evaluate::get_eval(board, color);
        if (eval > alpha) {
            alpha = eval;
        }

        MoveList forcedMoves = board.GenerateMoveList(color, true);
        forcedMoves.mvv_lva();

        auto moveIter = forcedMoves.Get().begin();
        while (moveIter != forcedMoves.Get().end() && alpha < beta) {
            board.makeMove(*moveIter);
            eval = -quies((color == WHITE) ? BLACK : WHITE, -beta, -alpha, board);
            board.undoMove();

            if (eval > alpha) {
                alpha = eval;
            }
            moveIter++;
        }
        return alpha;
    }

    int alpha_beta(int depth, int alpha, int beta, EColor color, Board& board)
    {
        if (depth == 0) {
            return quies(color, alpha, beta, board);
        }
        
        MoveList moves = board.GenerateMoveList(color);
        moves.mvv_lva();

        auto killerMove = std::find(moves.Get().begin(), moves.Get().end(), bestMoves[depth - 1]);
        if (killerMove != moves.Get().end()) {
            moves.replaceToPriorities(killerMove);
        }

        auto moveIter = moves.Get().begin();
        if (moveIter == moves.Get().end()) {
            return -INF;
        }
        Move bestMove = *moveIter;

        while (moveIter != moves.Get().end() && alpha < beta) {
            board.makeMove(*moveIter);
            int temp = INF;
            if (!board.IsGameOver()) {
                temp = -alpha_beta(depth - 1, -beta, -alpha, (color == WHITE) ? BLACK : WHITE, board);
            }
            board.undoMove();

            if (temp > alpha) {
                alpha = temp;
                bestMove = *moveIter;
                bestMoves[depth - 1] = bestMove;
            }
            moveIter++;
        }

        return alpha;
    }
}

namespace search
{
    search_struct get_best(Board board, EColor color, int depth)
    {

        bestMoves = std::vector<Move>(depth);
        int score = alpha_beta(depth, -INF, INF, color, board);
        search_struct ss = { bestMoves.back() };
        return ss;
    }
}
