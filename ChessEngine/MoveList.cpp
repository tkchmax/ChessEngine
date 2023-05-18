#include "MoveList.h"
#include "Misc.h"

class Transposition;

namespace {
    constexpr int HASH_MOVE_BONUS = 30000;
    constexpr int PV_MOVE_BONUS = 20000;
    constexpr int FIRST_KILLER_BONUS = 9000;
    constexpr int SECOND_KILLER_BONUS = 8000;
}

Move MoveList::operator[](int idx) const
{
    return list[idx].move;
}

MoveList::MoveList(const std::vector<Move>& moves)
{
    len = 0;
    for (const auto& m : moves) {
        add(m);
    }
}

int MoveList::get_score(int idx) const
{
    return list[idx].score;
}

void MoveList::add(const U32& move, int score)
{
    list[len++] = { move, score };
}

void MoveList::pick(int idx)
{
    int bestScore = 0;
    int bestIdx = idx;

    //"sorting", taking the best move up 
    //so that it can be picked during the search at the current index
    for (int i = idx; i < len; ++i) {
        if (list[i].score > bestScore) {
            bestScore = list[i].score;
            bestIdx = i;
        }
    }

    //swap
    s_move temp = list[idx];
    list[idx] = list[bestIdx];
    list[bestIdx] = temp;
}

void MoveList::sort(const KillerHeuristic& killers, const HistoryHeuristic& hist, const CounterMoveHeuristic& countHist, Move cacheMove, Move prevMove, Move pvMove, int ply, EColor color)
{
    for (int i = 0; i < size(); ++i) {
        if (list[i].move == cacheMove) {
            list[i].score += HASH_MOVE_BONUS;
        }
        else {
            if (list[i].move == pvMove) {
                list[i].score += PV_MOVE_BONUS;
            }
            else if (list[i].move == killers[0][ply]) {
                list[i].score += FIRST_KILLER_BONUS;
            }
            else if (list[i].move == killers[1][ply]) {
                list[i].score += SECOND_KILLER_BONUS;
            }
            else {
                int from = READ_FROM(list[i].move);
                int to = READ_TO(list[i].move);

                list[i].score += hist[color][from][to];

                if (list[i].move == countHist[READ_FROM(prevMove)][READ_TO(prevMove)]) {
                    list[i].score += 20;
                }
            }
        }
    }
}

std::string GetAlgebraicNotation(Move move)
{
    ESquare from = ESquare(READ_FROM(move));
    ESquare to = ESquare(READ_TO(move));
    EFigure fig = EFigure(READ_FIGURE(move));

    std::string notation = misc::ToString(from) + misc::ToString(to);

    switch (READ_MOVE_TYPE(move)) {
    case CASTLING: return to > from ? "O-O" : "O-O-O";
    case PROMOTION:
    {
        notation = misc::ToString(to) + "=";
        return notation += fig == QUEEN ? "Q" :
            fig == ROOK ? "R" :
            fig == BISHOP ? "B" :
            fig == KNIGHT ? "N" : " ";
    }
    }

    return notation;
}

std::ostream& operator<<(std::ostream& out, const MoveList& list)
{
    using namespace misc;
    out << "len: " << list.size() << std::endl;
    for (int i = 0; i < list.size(); ++i) {
        auto move = list[i];
        out << "From: " << ToString(ESquare(READ_FROM(move))) << std::endl;
        out << "To: " << ToString(ESquare(READ_TO(move))) << std::endl;
        out << "Figure to move: " << ToString(EFigureType(READ_FIGURE(move))) << std::endl;
        out << "Move type: " << ToString(EMoveType(READ_MOVE_TYPE(move))) << std::endl;
        out << "score: " << list.get_score(i) << std::endl;
        out << "U32: " << move << "\n\n";
    }
    return out;
}
