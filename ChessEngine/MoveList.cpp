#include "MoveList.h"

void MoveList::add(const Move& move, bool isPriorityMove)
{
    if (isPriorityMove) {
        moves.push_front(move);
        nPriority += 1;
    }
    else if (move.GetMoveType() == EMoveType::CAPTURE) {
        moves.insert(std::next(moves.begin(), nPriority), move);
        nCapture += 1;
    } 
    else {
        moves.push_back(move);
        nSilent += 1;
    }
}

