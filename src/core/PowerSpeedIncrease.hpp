#pragma once

#include "CoreLoader.hpp"

CGAPI class PowerSpeedIncrease: public Power
{
public:
    PowerSpeedIncrease();
    virtual void drawPower(RenderWindow* wnd);
    virtual void onPowerTick(int tick);
    virtual int getCost();
    virtual string getName();
    virtual bool isAntiPower();
};
