#include "PowerFreeze.hpp"
#include "GameDisplay.h"
#include "Game.h"


//// FREEZE ////

PowerFreeze::PowerFreeze() : Power() {}
void PowerFreeze::onPowerStop()
{
	Game::instance->setGameSpeed(Game::instance->getGameSpeed() * 3.f);
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
	Game::instance->setGameSpeed(Game::instance->getGameSpeed() / 3.f);
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

