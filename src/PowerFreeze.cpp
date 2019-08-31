#include "PowerFreeze.hpp"
#include "GameDisplay.h"
#include "Game.h"
#include <cmath>

//// FREEZE ////

PowerFreeze::PowerFreeze() : Power() {}
void PowerFreeze::onPowerStop()
{

}

void PowerFreeze::onPowerTick(int tc)
{
    if(Game::instance->powerMaxTime - tc < maxPowerTime / 18)
        Game::instance->setGameSpeed(Game::instance->getGameSpeed() / (0.005f * sqrt(currentLevel) + 1.f));
    else if(tc < maxPowerTime / 16)
        Game::instance->setGameSpeed(Game::instance->getGameSpeed() * (0.000725f * sqrt(currentLevel) + 1.f));
}

void PowerFreeze::drawPower(RenderWindow * wnd)
{
	int effectAlpha = 32;

	RectangleShape rs(Vector2f(1920.f, 500.f));
	rs.setPosition(0.f, GameDisplay::instance->getSize().y / 2);
	rs.setOrigin(0.f, 250.f);
	rs.setFillColor(Color(164, 241, 249, effectAlpha));
	wnd->draw(rs);
}

bool PowerFreeze::onPowerStart()
{
	//Game::instance->setGameSpeed(Game::instance->getGameSpeed() / (4.f * sqrt(currentLevel)));
	return true;
}
int PowerFreeze::getCost()
{
    return 1000;
}
string PowerFreeze::getName()
{
    return "freeze";
}

