#include "EFigure.h"

const char* ToString(EFigure figureName)
{
    switch (figureName)
    {
    case EFigure::PAWN: return "PAWN";
    case EFigure::KNIGHT: return "KNIGHT";
    case EFigure::BISHOP: return "BISHOP";
    case EFigure::ROOK: return "ROOK";
    case EFigure::QUEEN: return "QUEEN";
    case EFigure::KING: return "KING";
    case EFigure::NO_FIGURE: return "NO_FIGURE";
    default: return "ErrorFigureType";
    }
}
