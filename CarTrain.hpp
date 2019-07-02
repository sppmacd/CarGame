#pragma once

#include "Car.h"

class CarTrain : public Car
{
public:
    CarTrain(float speed, int lane);
    void onLeave(Game* game);
};
