#include "Move.h"

Move::Move(ESquare from, ESquare to, EFigure figure, EFigure capture, EMoveType move_type, EColor color)
{
    Move(ToInt(from), ToInt(to), ToInt(figure), ToInt(capture), ToInt(move_type), ToInt(color));
}

Move::Move(int from, int to, int figure, int capture, int move_type, int color)
{
    move = 0;
    move = WRITE_FROM(from, move) | WRITE_TO(to, move) | WRITE_FIGURE(figure, move) |
        WRITE_CAPTURE(capture, move) | WRITE_MOVE_TYPE(move_type, move) | WRITE_COLOR(color, move);
}

ESquare Move::GetFrom()
{
    return static_cast<ESquare>(READ_FROM(move));
}

ESquare Move::GetTo()
{
    return static_cast<ESquare>(READ_TO(move));
}

EFigure Move::GetFigure()
{
    return static_cast<EFigure>(READ_FIGURE(move));
}

EFigure Move::GetCapture()
{
    return static_cast<EFigure>(READ_CAPTURE(move));
}

EMoveType Move::GetMoveType()
{
    return static_cast<EMoveType>(READ_MOVE_TYPE(move));

}

EColor Move::GetMoveColor()
{
    return static_cast<EColor>(READ_COLOR(move));
}
