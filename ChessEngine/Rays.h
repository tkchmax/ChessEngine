#pragma once
#ifndef RAYS_H_
#define RAYS_H_

#include <array>
#include "EDirection.h"

class Rays
{
public:
    using RaysArray = std::array<std::array<std::uint64_t, 64>, 8>;

    static const Rays& Get();

    RaysArray GetRays() const { return rays; }
private:
    Rays();
    Rays(const Rays&) = delete;
    void operator=(const Rays&) = delete;
private:
    RaysArray rays;
};

#endif // !RAYS_H_

