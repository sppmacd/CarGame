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
    for(Car* car: Game::instance->cars)
    {
        if(abs(car->getSpeed()) > 15.f / sqrt(currentLevel))
        {
            car->setSpeed(car->getSpeed() - (0.5f * sqrt(currentLevel) + 1.f));
        }
    }
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

