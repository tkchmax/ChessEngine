#include "Search.h"
#include "Position.h"
#include "Evaluate.h"
#include "Transposition.h"
#include <iostream>
#include <chrono>
#include <memory>

namespace {
    struct Stats {
        Stats() :
            nNodes(0),
            killers(),
            hist(),
            counterHist(),
            table(new Transposition(256)) {}

        U64 nNodes;
        KillerHeuristic killers;
        HistoryHeuristic hist;
        CounterMoveHeuristic counterHist;
        std::unique_ptr<Transposition> table;
    } ss;

    int pvLength[64];
    Move pvTable[64][64];

    U64 repetitionTable[1000];
    int repetitionIndex;

    bool followPv = false;
    bool scorePv = false;

    void EnablePvScoring(const MoveList& list, int ply) {
        followPv = false;

        for (int i = 0; i < list.size(); ++i) {
            //make sure we hit pv line
            if (pvTable[0][ply] == list[i]) {
                followPv = true;
                scorePv = true;
                return;
            }
        }
    }

    //Quiescence Search
    int quies(int alpha, int beta, const Position& pos) {
        int eval = evaluate::eval(pos, pos.side_to_move());
        if (eval >= beta) {
            return beta;
        }
        if (eval > alpha) {
            alpha = eval;
        }

        //generate only captures
        MoveList forcedMoves;
        pos.generate<CAPTURES>(forcedMoves);

        for (int i = 0; i < forcedMoves.size(); ++i) {
            forcedMoves.pick(i);

            if (!pos.is_move_legal(forcedMoves[i])) {
                continue;
            }

            Position newPos = pos.make_move(forcedMoves[i]);

            ss.nNodes++;
            eval = -quies(-beta, -alpha, newPos);

            if (eval >= beta) {
                return beta;
            }
            if (eval > alpha) {
                alpha = eval;
            }
        }

        return alpha;
    }

    bool IsRepetition(const Position& currentPos) {
        for (int i = 0; i < repetitionIndex; ++i) {
            if (repetitionTable[i] == currentPos.get_zobrist()) {
                return true;
            }
        }
        return false;
    }

    Move prevMove = 0;
    int alpha_beta(int depth, int ply, int alpha, int beta, const Position& pos, bool doNull = true) {

        pvLength[ply] = ply;

        int bestMove = 0;

        EHashFlag hashf = HASH_ALPHA;

        //get information about current position from the transposition table
        const s_node* const cache = ss.table->get(pos.get_zobrist());

        //3 fold repetition rule
        //check if current position has already been before in the current search tree
        if (IsRepetition(pos)) {
            return 0;
        }

        //check if currently in pv node
        int pv_node = beta - alpha > 1;

        if (ply && cache->flag != HASH_EMPTY && cache->depth >= depth && pv_node == 0) {
            //The score in the hash table is written relative to the white side
            //Therefore, we need to make sure the stored score match the current side
            int score = pos.side_to_move() == WHITE ? cache->score : -cache->score;

            if (cache->flag == HASH_EXACT) {
                return score;
            }
            if (cache->flag == HASH_ALPHA && score <= alpha) {
                return alpha;
            }
            if (cache->flag == HASH_BETA && score >= beta) {
                return beta;
            }
        }

        if (depth == 0) {
            return quies(alpha, beta, pos);
        }

        bool inCheck = pos.is_king_attacked(pos.side_to_move());

        //if the king is under attack, search one depth deeper
        if (inCheck) {
            depth++;
        }

        //Null-move heuristic
        //The idea is to give the opponent a free shot,
        //and if current position is still so good that exceed beta,
        //we assume that we'd also exceed beta if we went and searched all of the moves.
        if (doNull && depth >= 3 && ply && !inCheck) {
            //create temporary position
            Position nullPos(pos);

            //delete en passant
            nullPos.set_en_passant(SQ_NONE);

            //literally give the opponent a second move in a row
            nullPos.set_side2move(~pos.side_to_move());

            //update hash
            nullPos.recalcZobrist();

            //do search with reduced depth
            int score = -alpha_beta(depth - 1 - 2, ply + 1, -beta, -beta + 1, nullPos, false);

            //if even after two moves in a row opponent our score
            //is still >= beta, then make cut off
            if (score >= beta) {
                return beta;
            }
        }

        //Razoring
        if (!pv_node && !inCheck && depth <= 3) {
            //static evaluation + bonus
            int score = evaluate::eval(pos, pos.side_to_move()) + 125;

            int newScore;
            //Fail low
            //This position was not good enough for us even after adding the bonus value to the score 
            if (score < beta) {
                if (depth == 1) {
                    newScore = quies(alpha, beta, pos);
                    //max(score, newScore)
                    return score > newScore ? score : newScore;
                }
            }

            //Add second bonus
            score += 175;

            if (score < beta && depth <= 2) {
                newScore = quies(alpha, beta, pos);
                if (newScore < beta) {
                    //max(score, newScore)
                    return score > newScore ? score : newScore;
                }
            }
        }

        MoveList moves;
        pos.generate<PSEUDO>(moves);

        //check if we are following the pv line and
        //if we should put pv move at the top of the list
        if (followPv) {
            EnablePvScoring(moves, ply);
        }
        Move pvMove = 0;
        if (scorePv) {
            pvMove = pvTable[0][ply];
            scorePv = false;
        }

        //updating the scores for the moves
        moves.sort(ss.killers,
            ss.hist,
            ss.counterHist,
            cache->bestMove,
            prevMove,
            pvMove,
            ply,
            pos.side_to_move());

        int nLegal = 0;
        for (int i = 0; i < moves.size(); ++i) {
            //lift the best move to the list[i]
            moves.pick(i);

            //make sure move is legal
            if (!pos.is_move_legal(moves[i])) {
                continue;
            }

            //save hash of the current position to determine 3fold repetition later.
            repetitionTable[repetitionIndex] = pos.get_zobrist();
            repetitionIndex++;

            Position newPos = pos.make_move(moves[i]);
            ss.nNodes++;
            nLegal++;
            prevMove = moves[i];

            int score;

            //Full window search for the first move in the move-list
            if (i == 0) {
                score = -alpha_beta(depth - 1, ply + 1, -beta, -alpha, newPos, doNull);
            }
            else {
                //Late move reductions. here we rely on the fact we have a good enough sorting of
                //moves to be sure that the first 4 moves will most often be the best, so we count
                //all other moves at a reduced depth and a smaller window to try speed up the search

                //Conditions to LMR
                if (i >= 3 && depth >= 3 && !inCheck
                    && !pos.is_capture_move(moves[i]) && EMoveType(READ_MOVE_TYPE(moves[i])) != PROMOTION
                    && !newPos.is_king_attacked(newPos.side_to_move()) && pvMove == 0 && pv_node == 0) {
                    int r = nLegal > 10 && depth >= 4 ? 2 : 1;
                    score = -alpha_beta(depth - 1 - r, ply + 1, -alpha - 1, -alpha, newPos, doNull);
                }
                else {
                    score = alpha + 1;
                }

                //Principal Variation Search. 
                //The the technique makes the assumption that if
                //you find one PV move when you are searching a node, you definitely have a PV node.
                //It again assumes that move ordering will be good enough that we won't find a better PV move
                //Once we've found a move with a score that is between alpha and beta, the rest of the moves
                //are searched with the goal of proving that they are all bad
                if (score > alpha) {
                    score = -alpha_beta(depth - 1, ply + 1, -alpha - 1, -alpha, newPos, doNull);
                    //Check for failure, re-search full window and full depth.
                    if (score > alpha && score < beta) {
                        score = -alpha_beta(depth - 1, ply + 1, -beta, -alpha, newPos, doNull);
                    }
                }
            }

            repetitionIndex--;

            //int score = -alpha_beta(depth - 1, ply + 1, -beta, -alpha, newPos, doNull);

            //current move improves score
            if (score > alpha) {
                alpha = score;

                hashf = HASH_EXACT;
                bestMove = moves[i];

                //History heuristic
                if (!pos.is_capture_move(bestMove)) {
                    ss.hist[pos.side_to_move()][READ_FROM(bestMove)][READ_TO(bestMove)] += depth;
                }

                //Reset triangular PV table with new best move
                pvTable[ply][ply] = bestMove;
                for (int nextPly = ply + 1; nextPly < pvLength[ply + 1]; ++nextPly) {
                    pvTable[ply][nextPly] = pvTable[ply + 1][nextPly];
                }
                pvLength[ply] = pvLength[ply + 1];
            }

            //Fails High
            if (score >= beta) {
                if (!pos.is_capture_move(moves[i])) {
                    //Save move for Hiller Heuristic
                    ss.killers[1][ply] = ss.killers[0][ply];
                    ss.killers[0][ply] = moves[i];

                    //History Heuristic
                    ss.hist[pos.side_to_move()][READ_FROM(moves[i])][READ_TO(moves[i])] = depth * depth;

                    //Countermove Heuristic
                    ss.counterHist[READ_FROM(prevMove)][READ_TO(prevMove)] = moves[i]; //!
                }

                //save position to hash table
                ss.table->add(pos.get_zobrist(), bestMove, depth, pos.side_to_move() == WHITE ? beta : -beta, HASH_BETA);

                return beta;
            }
        }

        //if there were no legal moves, check for mate or stalemate
        if (nLegal == 0) {
            return inCheck ? -figure::KING_COST + ply : 0;
        }

        //save position to hash table
        ss.table->add(pos.get_zobrist(), bestMove, depth, pos.side_to_move() == WHITE ? alpha : -alpha, hashf);

        //Fails low
        //meaning that none of the moves in here will be any good
        return alpha;
    }
}

namespace search {
    std::ostream& operator<<(std::ostream& out, const s_search& s) {
        out << "score cp " << s.score <<
            " nodes " << s.nNodes <<
            " time " << s.time <<
            " pv ";
        for (const Move& m : s.pv) {
            out << GetAlgebraicNotation(m) << " ";
        }
        out << std::endl;
        return out;
    }

    s_search search(int depth, const Position& pos) {
        ss = Stats();
        std::vector<Move> pv;

        auto start = std::chrono::high_resolution_clock::now();
        int score = alpha_beta(depth, 0, -INF, INF, pos);
        auto end = std::chrono::high_resolution_clock::now();

        auto timeMS = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        return { pv, timeMS, ss.nNodes, score };
    }

    int statsf = true;
    s_search iterative_deepening(int depth, const Position& pos) {
        ss = Stats();
        scorePv = false;
        followPv = false;
        std::vector<Move> pv;
        s_search res;

        //set aspiration window size
        int alpha = -INF;
        int beta = INF;

        //Iterative deepenig
        for (int d = 1; d <= depth; ++d) {
            ss.nNodes = 0;
            pv.clear();
            followPv = true;

            auto start = std::chrono::high_resolution_clock::now();

            //search
            int score = alpha_beta(d, 0, alpha, beta, pos);

            //if we failed with current aspiration window, re-search with full
            if (score <= alpha || score >= beta) {
                alpha = -INF;
                beta = INF;
                d--;
                continue;
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto timeMS = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

            //update aspiration window
            alpha = score - 50;
            beta = score + 50;

            //save pv line
            for (int i = 0; i < pvLength[0]; ++i) {
                pv.push_back(pvTable[0][i]);
            }
            res = { pv, timeMS, ss.nNodes, score };

            std::cout << "info depth " << d << " " << res;
        }
        return res;
    }

    U64 perft(const Position& pos, int depth)
    {
        U64 nodes = 0;
        if (depth == 0) {
            return 1;
        }

        MoveList moves;
        pos.generate<LEGAL>(moves);
        for (int i = 0; i < moves.size(); ++i) {
            Position newPos = pos.make_move(moves[i]);

            //if (newPos.get_zobrist() != Transposition::GetZobristHash(newPos)) {
            //    std::cout << "hash!\n";
            //    throw;
            //}

            nodes += perft(newPos, depth - 1);
        }

        return nodes;
    }
}