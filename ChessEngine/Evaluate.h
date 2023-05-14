#pragma once
#ifndef EVALUATE_H_
#define EVALUATE_H_
#include "Position.h"

class Position;

namespace evaluate {
    int eval(const Position& pos, EColor relTo);
    int calc_gamephase_score(const Position& pos);
}

#endif //EVALUATE_H_