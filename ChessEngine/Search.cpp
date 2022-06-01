#include "Search.h"
#include "Evaluate.h"
#include "Transposition.h"
#include <vector>
#include <iostream>

namespace
{
    int n = 0;
    std::vector<Move> bestMoves;
    std::unique_ptr<TranspositionTable> transpositionTable;

    int quies(EColor color, int alpha, int beta, Board& board)
    {
        int eval = evaluate::get_eval(board, color);
        if (eval > alpha) {
            alpha = eval;
        }

        MoveList forcedMoves = board.GenerateMoveList(color, true);
        forcedMoves.mvv_lva();
        transposition_struct currentPos = transpositionTable->get();
        if (!currentPos.empty()) {
            auto transpositionBestMove = std::find(forcedMoves.Get().begin(), forcedMoves.Get().end(), currentPos.best_move);
            if (transpositionBestMove != forcedMoves.Get().end()) {
                forcedMoves.replaceToPriorities(transpositionBestMove);
            }
        }

        auto moveIter = forcedMoves.Get().begin();
        while (moveIter != forcedMoves.Get().end() && alpha < beta) {
            board.makeMove(*moveIter);
            n++;
            transpositionTable->amend_hash(*moveIter);
            eval = -quies((color == WHITE) ? BLACK : WHITE, -beta, -alpha, board);
            board.undoMove();
            transpositionTable->amend_hash(*moveIter);

            if (eval > alpha) {
                alpha = eval;
                transpositionTable->add(transposition_struct(Move(*moveIter), alpha));
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

        transposition_struct currentPos = transpositionTable->get();
        if (!currentPos.empty()) {
            auto transpositionBestMove = std::find(moves.Get().begin(), moves.Get().end(), currentPos.best_move);
            if (transpositionBestMove != moves.Get().end()) {
                moves.replaceToPriorities(transpositionBestMove);
            }
        }

        auto moveIter = moves.Get().begin();
        if (moveIter == moves.Get().end()) {
            return -INF;
        }
        Move bestMove = *moveIter;

        while (moveIter != moves.Get().end() && alpha < beta) {
            board.makeMove(*moveIter);
            n++;
            transpositionTable->amend_hash(*moveIter);
            int temp = INF;
            if (!board.IsGameOver()) {
                temp = -alpha_beta(depth - 1, -beta, -alpha, (color == WHITE) ? BLACK : WHITE, board);
            }
            board.undoMove();
            transpositionTable->amend_hash(*moveIter);

            if (temp > alpha) {
                alpha = temp;
                bestMove = *moveIter;
                bestMoves[depth - 1] = bestMove;
                transpositionTable->add(transposition_struct(bestMove, alpha));
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
        n = 0;
        bestMoves = std::vector<Move>(depth);
        if (transpositionTable == nullptr) {
            transpositionTable = std::make_unique<TranspositionTable>(board);
        }
        else {
            transpositionTable->reset_current_hash(board);
        }
        
        int score = alpha_beta(depth, -INF, INF, color, board);
        std::cout << "N: " << n << std::endl;
        std::cout << "t_size: " << transpositionTable->get_size() << std::endl;
        return { bestMoves.back() };
    }
}
