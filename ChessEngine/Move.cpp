#include "Move.h"
#include <iostream>

Move::Move(int from, int to, int figure, int capture, int move_type, int color)
{
    move = 0;
    move = WRITE_FROM(from, move) | WRITE_TO(to, move) | WRITE_FIGURE(figure, move) |
        WRITE_CAPTURE(capture, move) | WRITE_MOVE_TYPE(move_type, move) | WRITE_COLOR(color, move);
}

U32 Move::Get() const
{
    return move;
}

ESquare Move::GetFrom() const
{
    return static_cast<ESquare>(READ_FROM(move));
}

ESquare Move::GetTo() const
{
    return static_cast<ESquare>(READ_TO(move));
}

EFigure Move::GetFigure() const
{
    return static_cast<EFigure>(READ_FIGURE(move));
}

EFigure Move::GetCapture() const
{
    return static_cast<EFigure>(READ_CAPTURE(move));
}

EMoveType Move::GetMoveType() const
{
    return static_cast<EMoveType>(READ_MOVE_TYPE(move));

}

EColor Move::GetMoveColor() const
{
    return static_cast<EColor>(READ_COLOR(move));
}

void Move::show() const
{
	std::cout << "From: " << ToString(GetFrom()) << std::endl;
	std::cout << "To: " << ToString(GetTo()) << std::endl;
	std::cout << "Figure to move: " << ToString(GetFigure()) << std::endl;
	std::cout << "Capture figure: " << ToString(GetCapture()) << std::endl;
	std::cout << "Move type: " << ToString(GetMoveType()) << std::endl;
	std::cout << "Color: " << ToString(GetMoveColor()) << std::endl;
    std::cout << "U32: " << move << std::endl;
    std::cout << std::endl;
}
