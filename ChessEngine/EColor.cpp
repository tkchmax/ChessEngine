#include "EColor.h"

const char* ToString(EColor color)
{
    switch (color)
    {
    case EColor::WHITE: return "WHITE";
    case EColor::BLACK: return "BLACK";
    default: return "ErrorColor";
    }
}
