#pragma once

#include <SFML/Graphics.hpp>
#include "Car.h"
using namespace sf;

// Some typedefs

/// Start power
typedef bool (*PowerStartHandler)();

/// Stop power / start cooldown
typedef void (*PowerStopHandler)();

/// Tick power
typedef void (*PowerTickHandler)(int);

/// Tick cooldown
typedef void (*CooldownTickHandler)(int);

/// Stop cooldown
typedef void (*CooldownStopHandler)();

/// Draw power
typedef void (*DrawPowerFunc)(RenderWindow* wnd);

class Power
{
public:
	static bool onPowerStart();
	static void onPowerStop();
	static void onPowerTick(int powerTick);
	static void onCooldownTick(int cooldownTick);
	static void onCooldownStop();
	static void drawPower(RenderWindow* wnd);
	static void drawPowerIdle(RenderWindow* wnd);
};

class PowerOil
{
public:
	static bool onPowerStart();
	static void onPowerStop();
	static void onPowerTick(int powerTick);
	static void drawPower(RenderWindow* wnd);
	static void drawPowerIdle(RenderWindow* wnd);

	static Vector2f pos;
	static bool isWeaken; //weaken on placed at center lane!
};

class PowerFreeze
{
public:
	static bool onPowerStart();
	static void onPowerStop();
	static void drawPower(RenderWindow* wnd);
};

struct PowerHandles
{
    DrawPowerFunc drawPower;
	PowerStartHandler onPowerStart;
	PowerStopHandler onPowerStop;
	PowerTickHandler onPowerTick;
	DrawPowerFunc drawPowerIdle;
	CooldownTickHandler onCooldownTick;
	CooldownStopHandler onCooldownStop;
	int maxPowerTime;

    PowerHandles(
	DrawPowerFunc _drawPower = Power::drawPower,
	PowerStartHandler _onPowerStart = Power::onPowerStart,
	PowerStopHandler _onPowerStop = Power::onPowerStop,
	PowerTickHandler _onPowerTick = Power::onPowerTick,
	DrawPowerFunc _drawPowerIdle = Power::drawPowerIdle,
	CooldownTickHandler _onCooldownTick = Power::onCooldownTick,
	CooldownStopHandler _onCooldownStop = Power::onCooldownStop)
	:drawPower(_drawPower)
	,onPowerStart(_onPowerStart)
	,onPowerStop(_onPowerStop)
	,onPowerTick(_onPowerTick)
	,drawPowerIdle(_drawPowerIdle)
	,onCooldownTick(_onCooldownTick)
	,onCooldownStop(_onCooldownStop)
	{
        setMaxTime(250);
	}

	PowerHandles& setMaxTime(int time)
	{
	    maxPowerTime = time;
	    return *this;
	}
};
