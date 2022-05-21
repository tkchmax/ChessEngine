#pragma once
#ifndef MOVELIST_H_
#define MOVELIST_H_

#include <list>
#include "Move.h"

class MoveList
{
public:
    void add(const Move& move, bool isPriorityMove=false);

    const std::list<Move>& GetList() { return moves; }

   private:
    std::list<Move> moves;
    int nPriority, nCapture, nSilent;
};

#endif // !MOVELIST_H_


