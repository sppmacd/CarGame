#pragma once

#include "Car.h"
#include "Game.h"

class CarBomb : public Car
{
public:
    CarBomb(float spd, int lane);
    void onDestroy(Game* game);
    void onLeave(Game* game);
};
