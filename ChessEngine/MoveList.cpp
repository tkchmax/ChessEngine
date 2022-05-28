#include "MoveList.h"
#include <iostream>

void MoveList::add(const Move& move, bool isPriorityMove)
{
    if (isPriorityMove) {
        addPriority_(move);
    }
    else if (move.GetMoveType() == EMoveType::CAPTURE) {
        addCapture_(move);
    }
    else {
        if (IsPawnTransformType(move.GetMoveType()) && move.GetCapture() != EFigure::NO_FIGURE) {
            addCapture_(move);
        }
        else {
            addSilent_(move);
        }
     }
}

void MoveList::operator+=(const MoveList& other)
{
    for (auto moveIter = other.moves.begin(); moveIter != other.moves.end(); ++moveIter) {
        add(*moveIter);
    }
}

void MoveList::replaceToPriorities(std::list<Move>::const_iterator moveIter)
{
    Move temp = *moveIter;
    moves.erase(moveIter);
    add(temp, true);
}

void MoveList::mvv_lva()
{
    auto captures_begin = std::next(moves.end(), -nSilent - nCapture);
    auto captures_end = std::next(moves.end(), -nSilent);

    std::list<Move> sorted_captures;
    sorted_captures.splice(sorted_captures.begin(), moves, captures_begin, captures_end);

    sorted_captures.sort([](const Move& first, const Move& second)
        {
            int first_score = first.GetCapture() - first.GetFigure();
            int second_score = second.GetCapture() - second.GetFigure();
            return first_score > second_score;
        });

    moves.splice(captures_end, sorted_captures);
}

void MoveList::addSilent_(const Move& move)
{
    moves.push_back(move);
    nSilent += 1;
}

void MoveList::addCapture_(const Move& move)
{
    moves.insert(std::next(moves.begin(), nPriority), move);
    nCapture += 1;
}

void MoveList::addPriority_(const Move& move)
{
    moves.push_front(move);
    nPriority += 1;
}

std::ostream& operator<<(std::ostream& out, const MoveList& moveList)
{
    out << "priority_count:" << moveList.nPriority << std::endl;
    out << "silent_count:" << moveList.nSilent << std::endl;
    out << "capture_count:" << moveList.nCapture << std::endl;
    out << "moves size: " << moveList.moves.size() << std::endl;
    out << std::endl;

    for (auto move = moveList.moves.begin(); move != moveList.moves.end(); ++move) {
        (*move).show();
    }

    return out;
}
