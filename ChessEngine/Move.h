#pragma once
#ifndef MOVE_H_
#define MOVE_H_

#include "Misc.h"
#include "EMoveType.h"
#include "EColor.h"
#include "EFigure.h"
#include "ESquare.h"

class Move
{
public:
    Move() : move(0) {}
    Move(ESquare from, ESquare to, EFigure figure, EFigure capture, EMoveType move_type, EColor color);
    Move(int from, int to, int figure, int capture, int move_type, int color);

    ESquare GetFrom();
    ESquare GetTo();
    EFigure GetFigure();
    EFigure GetCapture();
    EMoveType GetMoveType();
    EColor GetMoveColor();
    
private: 
    U32 move;
};

#endif //!MOVE_H_
