#pragma once

#include "CoreLoader.hpp"

class CarBomb : public Car
{
public:
    CarBomb(float spd, int lane);
    void onDamage(Game* game);
    void onLeave(Game* game);
};
