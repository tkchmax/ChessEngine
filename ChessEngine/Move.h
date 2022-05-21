#pragma once
#ifndef MOVE_H_
#define MOVE_H_

#include "Misc.h"
#include "EMoveType.h"
#include "EColor.h"
#include "EFigure.h"
#include "ESquare.h"
#include "EDirection.h"

class Move
{
public:
    Move() : move(0) {}
    Move(int from, int to, int figure, int capture, int move_type, int color);

    ESquare GetFrom() const;
    ESquare GetTo() const;
    EFigure GetFigure() const;
    EFigure GetCapture() const;
    EMoveType GetMoveType() const;
    EColor GetMoveColor() const;
    
private: 
    U32 move;
};

#endif //!MOVE_H_
