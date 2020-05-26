#pragma once

#include "CoreLoader.hpp"

CGAPI class PowerDamageDecrease: public Power
{
public:
    PowerDamageDecrease();
    virtual void drawPower(RenderWindow* wnd);
    virtual bool onPowerStart();
    virtual void onPowerStop();
    virtual int getCost();
    virtual string getName();
    virtual bool isAntiPower();
};

