#pragma once
enum class EDirection
{
    NORTH,
    WEST,
    SOUTH,
    EAST,
    NORTH_EAST,
    NORTH_WEST,
    SOUTH_WEST,
    SOUTH_EAST,
    SIZE
};

int ToInt(EDirection);

