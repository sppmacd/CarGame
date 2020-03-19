#pragma once

#include "Power.h"

class PowerDamageDecrease: public Power
{
public:
    PowerDamageDecrease();
    virtual void drawPower(RenderWindow* wnd);
    virtual void onPowerTick(int tick);
    virtual int getCost();
    virtual string getName();
};

