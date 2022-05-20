#include <iostream>
#include "Rays.h"
#include "Misc.h"

using namespace std;

int main()
{
    auto rays = Rays::Get().GetRays();
    for (int i = 0; i < 8; ++i)
        misc::ShowBits(rays[ToInt(EDirection::NORTH)][i]);
}

