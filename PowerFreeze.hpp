#pragma once

#include "Power.h"

class PowerFreeze : public Power
{
public:
    PowerFreeze();
	virtual bool onPowerStart();
	virtual void onPowerStop();
	virtual void drawPower(RenderWindow* wnd);
	virtual int getCost();
    virtual string getName();
};
