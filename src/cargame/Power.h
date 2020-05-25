#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "GameplayObject.hpp"

using namespace std;
using namespace sf;

class Power : public GameplayObject
{
protected:
    int currentLevel;
public:
    int maxPowerTime;
    int cooldownTime;
    int id;

    // Default constructor
    Power(std::string modName);

    // Destructor
    virtual ~Power() {}

    // Sets max power time. This does not include abilities.
    virtual Power& setMaxTime(int time);

    // Sets power level that player currently has.
    void setLevel(int level);

    // Called when the power is used or started in any way.
	virtual bool onPowerStart();

	// Called when the power time runs out but before cooldown is started.
	virtual void onPowerStop();

	// Called every tick when the power is running.
	virtual void onPowerTick(int powerTick);

	// Called every tick when the cooldown is running.
	virtual void onCooldownTick(int cooldownTick);

	// Called when the power cooldown finishes.
	virtual void onCooldownStop();

	// Called when game is started, after power registering.
	virtual void onInit();

	// Called when new level is created.
	virtual void onLevelLoad();

	// Called when power textures are loaded. Here power can load its custom textures.
	virtual void onTextureLoad();

	// Called every render frame, when the power is running.
	virtual void drawPower(RenderWindow* wnd);

	// Called every render frame, if Player has selected the power but it's not running.
	virtual void drawPowerIdle(RenderWindow* wnd);

	// Returns base cost of power (at first level)
	virtual int getCost();

	// Returns unlocalized name of power. It's used by language files.
	virtual string getName();

	// Returns current player power level, basing on power ID.
	static int getCurrentPowerLevel();
};
