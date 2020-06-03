#include "PowerSpeedIncrease.hpp"
#include <cargame/Game.h>
#include "CoreLoader.hpp"

PowerSpeedIncrease::PowerSpeedIncrease()
    : Power()
{
    setMaxTime(500);
    cooldownTime = 0;
}
void PowerSpeedIncrease::drawPower(RenderWindow* wnd)
{
    RectangleShape rs(Vector2f(1920.f, 1080.f));
    rs.setFillColor(Color(255, 0, 0, min(128, Game::instance->powerTime * 3)));
    wnd->draw(rs);
}
void PowerSpeedIncrease::onPowerTick(int tick)
{
    //Game::instance->setGameSpeed(Game::instance->getGameSpeed() + 0.05f);
    // todo: give big acc to cars
    for(Car* car: Game::instance->cars)
    {
        car->setSpeed(car->getSpeed() + 100.f);
    }
}
int PowerSpeedIncrease::getCost()
{
    return 0;
}
string PowerSpeedIncrease::getName()
{
    return "speed_increase";
}
bool PowerSpeedIncrease::isAntiPower()
{
    return true;
}
