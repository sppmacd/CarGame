#include "PowerSpeedIncrease.hpp"
#include "Game.h"

PowerSpeedIncrease::PowerSpeedIncrease(): Power()
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
