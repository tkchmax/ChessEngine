#include "EMoveType.h"
#include <cassert>

int ToInt(EMoveType move_type)
{
    assert(move_type != EMoveType::SIZE);
    return static_cast<int>(move_type);
}

std::string ToString(EMoveType move_type)
{
    assert(move_type != EMoveType::SIZE);
    switch (move_type)
    {
        case EMoveType::SILENT: return "SILENT_MOVE";
        case EMoveType::CAPTURE: return "CAPTURE_MOVE";
        case EMoveType::SHORT_CASTLING: return "SHORT_CASTLING";
        case EMoveType::LONG_CASTLING: return "LONG_CASTLING";
        case EMoveType::TWO_SQUARE_PAWN_MOVE: return "TWO_SQUARE_PAWN_MOVE";
        case EMoveType::PAWN_TO_KNIGHT: return "PAWN_TO_KNIGHT";
        case EMoveType::PAWN_TO_BISHOP: return "PAWN_TO_BISHOP";
        case EMoveType::PAWN_TO_QUEEN: return "PAWN_TO_QUEEN";
        default: return "ERROR TYPE";
    }
}
