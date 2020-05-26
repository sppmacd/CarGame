#pragma once

#include "CoreLoader.hpp"

CGAPI class CarTrain : public Car
{
public:
    CarTrain(float speed, int lane);
    void onLeave(Game* game);
};
