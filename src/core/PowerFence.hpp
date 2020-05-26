#pragma once

#include "CoreLoader.hpp"

CGAPI class PowerFence : public Power
{
public:
    PowerFence();
	virtual bool onPowerStart();
	virtual void onPowerStop();
	virtual void onPowerTick(int powerTick);
	virtual void drawPower(RenderWindow* wnd);
	virtual void drawPowerIdle(RenderWindow* wnd);
	virtual int getCost();
    virtual string getName();

	Vector2f pos;
	int lane;
};
