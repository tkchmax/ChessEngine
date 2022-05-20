#include "EDirection.h"
#include <cassert>
int ToInt(EDirection direction)
{
    assert(direction != EDirection::SIZE);
    return static_cast<int>(direction);
  
}
