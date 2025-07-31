#pragma once
#ifndef EVALUATE_H_
#define EVALUATE_H_
#include "Position.h"

class Position;

namespace evaluate {
    int eval(const Position& pos, EColor relTo);
}

#endif //EVALUATE_H_