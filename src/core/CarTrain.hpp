#pragma once

#include "CoreLoader.hpp"

class CarTrain : public Car
{
public:
    CarTrain(float speed, int lane);
    void onLeave(Game* game);
};
