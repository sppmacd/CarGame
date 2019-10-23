#pragma once

#include "Power.h"

class PowerSpeedIncrease: public Power
{
public:
    PowerSpeedIncrease();
    virtual void drawPower(RenderWindow* wnd);
    virtual void onPowerTick(int tick);
    virtual int getCost();
    virtual string getName();
};
