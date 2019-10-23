#include "PowerSpeedIncrease.hpp"
#include "Game.h"

PowerSpeedIncrease::PowerSpeedIncrease(): Power()
{
    setMaxTime(500);
    cooldownTime = 0;
}
void PowerSpeedIncrease::drawPower(RenderWindow* wnd)
{
    RectangleShape rs(Vector2f(wnd->getSize()));
    rs.setFillColor(Color(255, 0, 0, 200));
    wnd->draw(rs);
}
void PowerSpeedIncrease::onPowerTick(int tick)
{
    Game::instance->setGameSpeed(Game::instance->getGameSpeed() + 0.05f);
}
int PowerSpeedIncrease::getCost()
{
    return 0;
}
string PowerSpeedIncrease::getName()
{
    return "speed_increase";
}
