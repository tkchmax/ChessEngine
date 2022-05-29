#pragma once
#ifndef RAYS_H_
#define RAYS_H_

#include <array>
#include "Constants.h"
#include "EDirection.h"
#include "ESquare.h"

class Rays
{
public:
    using RaysArray = std::array<std::array<U64, 64>, 8>;

    static const Rays& Get();

    const RaysArray& GetRays() const { return rays; }

private:
    Rays();
    Rays(const Rays&) = delete;
    void operator=(const Rays&) = delete;

private:
    RaysArray rays;
};

#endif // !RAYS_H_

