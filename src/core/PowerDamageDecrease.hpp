#pragma once

#include "CoreLoader.hpp"

class PowerDamageDecrease: public Power
{
public:
    PowerDamageDecrease();
    virtual void drawPower(RenderWindow* wnd);
    virtual bool onPowerStart();
    virtual void onPowerStop();
    virtual int getCost();
    virtual string getName();
};

