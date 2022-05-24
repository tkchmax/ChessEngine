#include "Move.h"
#include <iostream>

namespace
{
    std::string GetFigureNotationSymbol(const Move& move)
    {
        switch (move.GetFigure())
        {
        case PAWN: return "";
        case KNIGHT: return "N";
        case BISHOP: return "B";
        case ROOK: return "R";
        case QUEEN: return "Q";
        case KING: return "K";
        }
    }
}

Move::Move(int from, int to, int figure, int capture, int move_type, int color)
{
    move = 0;
    move = WRITE_FROM(from, move) | WRITE_TO(to, move) | WRITE_FIGURE(figure, move) |
        WRITE_CAPTURE(capture, move) | WRITE_MOVE_TYPE(move_type, move) | WRITE_COLOR(color, move);
}

std::string Move::GetNotation(bool isExpandedNotationNeeded) const
{
    std::string figure_symbol = "";
    std::string figure_capture_symbol = "";
    std::string capture_symbol = "";

    std::stringstream notation;
    
    if (GetFigure() == EFigure::PAWN && GetMoveType() == EMoveType::CAPTURE) {
        return std::string(1, ToString(GetFrom())[0]) + "x" + ToString(GetTo());
    }

    switch (GetMoveType())
    {
    case PAWN_TO_KNIGHT:  return std::string(ToString(GetTo())) + "=N";
    case PAWN_TO_BISHOP:  return std::string(ToString(GetTo())) + "=B";
    case PAWN_TO_ROOK:    return std::string(ToString(GetTo())) + "=R";
    case PAWN_TO_QUEEN:   return std::string(ToString(GetTo())) + "=Q";
    case SHORT_CASTLING:  return "O-O";
    case LONG_CASTLING:   return "O-O-O";
    case CAPTURE:
        capture_symbol = "x";
        break;
    }

    figure_symbol = GetFigureNotationSymbol(move);
    notation << figure_symbol;
    if (isExpandedNotationNeeded) {
        notation << (ToString(GetFrom()));
    }
    notation << capture_symbol << ToString(GetTo());
    return notation.str();
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

bool operator==(const Move& lsh, const Move& rsh)
{
    return lsh.move == rsh.move;
}
