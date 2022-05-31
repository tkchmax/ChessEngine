#pragma once
#ifndef MOVE_H_
#define MOVE_H_

#include "Misc.h"
#include "EMoveType.h"
#include "EColor.h"
#include "EFigure.h"
#include "ESquare.h"
#include "EDirection.h"
#include <sstream>

class Move
{
public:
    friend bool operator==(const Move& lsh, const Move& rsh);

    Move() : move(0) {}
    Move(int from, int to, int figure, int capture, int move_type, int color);
    Move(int move) : move(move) {}

    std::string GetNotation(bool isExpandedNotationNeeded=false) const;

    static Move GetWhiteShortCastlingMove() { return Move(moves::WHITE_SHORTCASTLING); }
    static Move GetBlackShortCastlingMove() { return Move(moves::BLACK_SHORTCASTLING); }
    static Move GetWhiteLongCastlingMove() { return Move(moves::WHITE_LONGCASTLING); }
    static Move GetBlackLongCastlingMove() { return Move(moves::BLACK_LONGCASTLING); }

    U32 Get() const;
    ESquare GetFrom() const;
    ESquare GetTo() const;
    EFigure GetFigure() const;
    EFigure GetCapture() const;
    EMoveType GetMoveType() const;
    EColor GetMoveColor() const;

    void show() const;
private:
    U32 move;
};

#endif //!MOVE_H_
