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
	DrawPowerFunc drawPower = Power::drawPower;
	PowerStartHandler onPowerStart = Power::onPowerStart;
	PowerStopHandler onPowerStop = Power::onPowerStop;
	PowerTickHandler onPowerTick = Power::onPowerTick;
	DrawPowerFunc drawPowerIdle = Power::drawPowerIdle;
	CooldownTickHandler onCooldownTick = Power::onCooldownTick;
	CooldownStopHandler onCooldownStop = Power::onCooldownStop;
};