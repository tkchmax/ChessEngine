#include "EColor.h"
#include <cassert>

int ToInt(EColor color)
{
    assert(color != EColor::SIZE);
    return static_cast<int>(color);
}
