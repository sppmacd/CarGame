#pragma once

#include "CoreLoader.hpp"

CGAPI class PowerPointBoost: public Power
{
public:
    PowerPointBoost();
    virtual void drawPower(RenderWindow* wnd);
    virtual void onPowerTick(int tick);
    virtual int getCost();
    virtual string getName();
};
