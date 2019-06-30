#include "Power.h"

Power::Power()
{

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

void Power::onPowerTick(int powerTick) {}

void Power::onCooldownTick(int cooldownTick) {}

void Power::onCooldownStop() {}

void Power::drawPower(RenderWindow * wnd) {}

void Power::drawPowerIdle(RenderWindow * wnd) {}
