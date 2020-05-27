#include "Power.h"
#include "Game.h"
#include "ModuleManager.hpp"

Power::Power() : GameplayObject(ModuleManager::instance->current())
{
    cooldownTime = 1800; //30 seconds
}
Power& Power::setMaxTime(int time)
{
    maxPowerTime = time;
    return *this;
}
bool Power::onPowerStart()
{
	return true;
}

void Power::onPowerStop() {}
void Power::onPowerTick(int) {}
void Power::onCooldownTick(int) {}
void Power::onCooldownStop() {}
void Power::drawPower(RenderWindow*) {}
void Power::drawPowerIdle(RenderWindow*) {}
void Power::onInit() {}
void Power::onTextureLoad() {}
void Power::onLevelLoad() {}
bool Power::isAntiPower() {return false; }

int Power::getCost()
{
    return 0;
}
string Power::getName()
{
    return "none";
}
int Power::getCurrentPowerLevel()
{
    return Game::instance->playerData.powerLevels[Game::instance->getCurrentPower()]->getLevel();
}

void Power::setLevel(int level)
{
    currentLevel = level;
}
