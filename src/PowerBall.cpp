#include "PowerBall.hpp"
#include "Car.h"
#include "GameDisplay.h"
#include "Game.h"
#include "Level.h"
#include <cmath>

PowerBall::PowerBall() : Power() {}
void PowerBall::onPowerStop()
{
	//Game::instance->setGameSpeed(Game::instance->getGameSpeed() / (1.3f / ((-1 / currentLevel) + 1)));
}

void PowerBall::PbBall::update()
{
    pos -= 1.5f;

    // check if will damage some cars
    for(Car* c : Game::instance->cars)
    {
        if(abs(c->getScreenPos().x - pos) < 82.f && c->getLine() == lane)
        {
            c->makeDestroy(0.1f);
            destroyAnim = 2;

            // push back car
            // TODO
        }
    }
}

bool PowerBall::onPowerStart()
{
	//Game::instance->setGameSpeed(Game::instance->getGameSpeed() * (1.3f / ((-1 / currentLevel) + 1)));
	return true;
}

void PowerBall::onPowerTick(int powerTick)
{
    // TODO
}

void PowerBall::drawPower(RenderWindow * wnd)
{
    // TODO
}

void PowerBall::drawPowerIdle(RenderWindow* wnd)
{
    // TODO
}
int PowerBall::getCost()
{
    return 8000;
}
string PowerBall::getName()
{
    return "ball";
}
