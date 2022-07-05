#include "Search.h"
#include "Evaluate.h"
#include "Transposition.h"
#include <algorithm>
#include <chrono>

namespace
{
    int nNodes = 0;
    std::vector<std::vector<Move>> killerMoves;
    std::unique_ptr<TranspositionTable> transpositionTable;

    std::vector<Move> prevPV;

    int quies(EColor color, int alpha, int beta, Board& board)
    {
        int eval = evaluate::get_eval(board, color);
        if (eval >= beta) {
            return beta;
        }
        if (eval > alpha) {
            alpha = eval;
        }

        MoveList forcedMoves = board.GenerateMoveList(color, true);
        forcedMoves.mvv_lva();

        auto moveIter = forcedMoves.Get().begin();
        while (moveIter != forcedMoves.Get().end() && alpha < beta) {
            board.makeMove(*moveIter);
            nNodes++;
            eval = -quies((color == WHITE) ? BLACK : WHITE, -beta, -alpha, board);
            board.undoMove();

            if (eval >= beta) {
                return beta;
            }
            if (eval > alpha) {
                alpha = eval;
            }
            moveIter++;
        }
        return alpha;
    }

    int alpha_beta(int depth, int ply, int alpha, int beta, EColor color, Board& board, std::vector<Move>& pv, bool DoNull = true)
    {
        if (depth == 0) {
            pv.clear();
            int eval = quies(color, alpha, beta, board);
            return eval;
        }
        std::vector<Move> childPV;
        EFlag hashf = EFlag::ALPHA;

        transposition_struct cache = transpositionTable->get();
        //if (!cache.empty() && cache.depth >= depth && ply > 0)
        //{
        //    if (cache.flag == EFlag::EXACT)
        //    {
        //        return cache.score;
        //    }
        //    if (cache.flag == EFlag::ALPHA && cache.score <= alpha)
        //    {
        //        return alpha;
        //    }
        //    if (cache.flag == EFlag::BETA && cache.score >= beta) {
        //        return beta;
        //    }
        //}

        //if (!cache.empty() && cache.depth >= depth && ply > 0)
        //{
        //    int score = cache.score;
        //    switch (cache.flag) {
        //    case EFlag::EXACT:
        //    case EFlag::BETA:
        //        if (score > alpha) alpha = score;
        //        if (alpha >= beta) return beta;
        //        break;
        //    case EFlag::ALPHA:
        //        if (score <= alpha) return alpha;
        //        break;
        //    }
        //}


        //Null-move
        if (DoNull && depth >= 3 && ply && !board.IsKingAttacked(color)) {
            std::vector<Move> t;
            int temp = -alpha_beta(depth - 1 - 2, ply + 1, -beta, -beta + 1, (color == WHITE) ? BLACK : WHITE, board, childPV, false);
            if (temp >= beta) {
                return beta;
            }
        }

        MoveList moves = board.GenerateMoveList(color);
        moves.mvv_lva();

        auto tryFirst = [&](Move move) {
            auto m = std::find(moves.Get().begin(), moves.Get().end(), move);
            if (m != moves.Get().end()) {
                moves.replaceToPriorities(m);
            }
        };

        tryFirst(killerMoves[1][ply]);
        tryFirst(killerMoves[0][ply]);

        if (cache.flag != EFlag::ALPHA && !cache.pv.empty()) {
            tryFirst(cache.pv[0]);
        }

        auto moveIter = moves.Get().begin();

        if (moveIter == moves.Get().end()) {
            if (!board.IsKingAttacked(color))
                return 0;
            return -cost_default::KING;
        }

        while (moveIter != moves.Get().end()) {
            board.makeMove(*moveIter);
            nNodes++;
            transpositionTable->amend_hash(*moveIter);
            //int temp;
            //if (moveIter == moves.Get().begin()) {
            //    temp = -alpha_beta(depth - 1, ply + 1, -beta, -alpha, (color == WHITE) ? BLACK : WHITE, board, childPV);
            //}
            //else {
            //    auto t = childPV;
            //    temp = -alpha_beta(depth - 1, ply + 1, -alpha-1, -alpha, (color == WHITE) ? BLACK : WHITE, board, t);
            //    if (temp > alpha && temp < beta) {
            //        temp = -alpha_beta(depth - 1, ply + 1, -beta, -temp, (color == WHITE) ? BLACK : WHITE, board, childPV);
            //    }
            //}
            int temp = -alpha_beta(depth - 1, ply + 1, -beta, -alpha, (color == WHITE) ? BLACK : WHITE, board, childPV);
            board.undoMove();
            transpositionTable->amend_hash(*moveIter);

            if (temp >= beta) {
                if ((*moveIter).GetMoveType() != EMoveType::CAPTURE) {
                    killerMoves[1][ply] = killerMoves[0][ply];
                    killerMoves[0][ply] = *moveIter;
                }

                transpositionTable->add({ *moveIter }, EFlag::BETA, depth, beta);

                return beta;
            }

            if (temp > alpha) {
                alpha = temp;
                hashf = EFlag::EXACT;

                pv.clear();
                pv.push_back(*moveIter);
                std::copy(childPV.begin(), childPV.end(), std::back_inserter(pv));
            }

            moveIter++;
        }
        transpositionTable->add(pv, hashf, depth, alpha);

        return alpha;
    }
}

namespace search
{
    search_struct get_best(Board board, EColor color, int depth)
    {
        nNodes = 0;
        killerMoves = std::vector<std::vector<Move>>(2, std::vector<Move>(depth));

        transpositionTable = std::make_unique<TranspositionTable>(board);

        std::vector<Move> pv;
        int score = alpha_beta(depth, 0, -INF, INF, color, board, pv);
        pv = transpositionTable->get().pv;

        search_struct ss = { pv, score, nNodes };
        ss.show();
        return ss;
    }

    search_struct iterative_deepening(Board board, EColor color, int time) {
        search_struct ss;

        transpositionTable = std::make_unique<TranspositionTable>(board);

        std::vector<Move> pv;

        auto start = std::chrono::high_resolution_clock::now();

        int alpha = -INF;
        int beta = INF;
        for (int depth = 1; depth <= 8; ++depth) {
            nNodes = 0;
            killerMoves = std::vector<std::vector<Move>>(2, std::vector<Move>(depth));

            int score = alpha_beta(depth, 0, alpha, beta, color, board, pv);
            if ((score <= alpha) || (score >= beta)) {
                std::cout << "out\n";
                alpha = -INF;
                beta = INF;
                depth--;
                continue;
            }
            alpha = score - 50;
            beta = score + 50;

            ss = { pv, score, nNodes };
            prevPV = pv;
            ss.show();

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> diff = end - start;

            /*if (diff.count() > 10 || abs(score) >= cost_default::KING) {
              break;
            }*/
            std::cout << std::endl;
        }
        std::cout << "\n-----\n";
        return ss;
    }
}