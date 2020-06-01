#include "PowerFreeze.hpp"
#include "CoreLoader.hpp"
#include <cmath>

//// FREEZE ////

PowerFreeze::PowerFreeze()
    : Power() {}
void PowerFreeze::onPowerStop()
{

}

void PowerFreeze::onPowerTick(int tc)
{
    if(Game::instance->maxPowerTime - tc < Game::instance->maxPowerTime / 18)
        Game::instance->setGameSpeed(Game::instance->getGameSpeed() / (0.005f * sqrt(currentLevel) + 1.f));
    else if(tc < Game::instance->maxPowerTime / 16)
        Game::instance->setGameSpeed(Game::instance->getGameSpeed() * (0.000725f * sqrt(currentLevel) + 1.f));
}

void PowerFreeze::drawPower(RenderWindow * wnd)
{
	int effectAlpha = 32;

	RectangleShape rs(Vector2f(1920.f, 1080.f));
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

