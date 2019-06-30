#pragma once

#include <SFML/Graphics.hpp>
#include <string>

using namespace std;
using namespace sf;

class Power
{
public:
    int maxPowerTime;

    Power();
    virtual ~Power() {}
    virtual Power& setMaxTime(int time);
	virtual bool onPowerStart();
	virtual void onPowerStop();
	virtual void onPowerTick(int powerTick);
	virtual void onCooldownTick(int cooldownTick);
	virtual void onCooldownStop();
	virtual void drawPower(RenderWindow* wnd);
	virtual void drawPowerIdle(RenderWindow* wnd);
	virtual int getCost();
	virtual string getName();
};
