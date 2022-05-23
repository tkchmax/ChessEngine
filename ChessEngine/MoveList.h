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
    
    void mvv_lva();
   private:
    std::list<Move> moves;
    int nPriority, nCapture, nSilent;
};

#endif // !MOVELIST_H_


