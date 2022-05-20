#pragma once

#ifndef MISC_H_
#define MISC_H_

#include "Constants.h"

#define WRITE_FROM(from,num) (num|from)
#define WRITE_TO(to,num) (num|(to<<6))
#define WRITE_FIGURE(figure,num) (num|(figure<<12))
#define WRITE_CAPTURE(capture,num) (num|(capture<<15))
#define WRITE_MOVE_TYPE(mt, num) (num|(mt<<18))
#define WRITE_COLOR(color, num) (num|(color<<22))

#define READ_FROM(num) (num&63)
#define READ_TO(num) ((num>>6)&63)
#define READ_FIGURE(num) ((num>>12)&7)
#define READ_CAPTURE(num) ((num>>15)&7)
#define READ_MOVE_TYPE(num) ((num>>18)&15)
#define READ_COLOR(num) ((num>>22)& 1)

namespace misc
{
    void ShowBits(const U64& bitboard);
    unsigned long BitScanForward(const U64& mask);
    unsigned long BitScanReverse(const U64& mask);
}

#endif // !MISC_H_

