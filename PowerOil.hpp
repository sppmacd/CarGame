#pragma once

#include "Power.h"

class PowerOil : public Power
{
public:
    PowerOil();
	virtual bool onPowerStart();
	virtual void onPowerStop();
	virtual void onPowerTick(int powerTick);
	virtual void drawPower(RenderWindow* wnd);
	virtual void drawPowerIdle(RenderWindow* wnd);

	Vector2f pos;
	int lane;

};
