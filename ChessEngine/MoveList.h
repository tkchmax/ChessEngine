#pragma once
#ifndef MOVELIST_H_
#define MOVELIST_H_

#include <list>
#include "Move.h"

class MoveList
{
public:
    friend std::ostream& operator<<(std::ostream& out, const MoveList& moveList);

    void add(const Move& move, bool isPriorityMove=false);
    void operator+=(const MoveList& other);

    const std::list<Move>& Get() { return moves; }
    void replaceToPriorities(std::list<Move>::const_iterator moveIter);
    void mvv_lva();

private:
    void addSilent_(const Move& move);
    void addCapture_(const Move& move);
    void addPriority_(const Move& move);
private:
    std::list<Move> moves;
    int nPriority, nCapture, nSilent;
};

#endif // !MOVELIST_H_


