#include "PowerDamageDecrease.hpp"
#include "Game.h"

PowerDamageDecrease::PowerDamageDecrease(): Power()
{
    setMaxTime(500);
    cooldownTime = 0;
}
void PowerDamageDecrease::drawPower(RenderWindow* wnd)
{
    RectangleShape rs(Vector2f(wnd->getSize()));
    rs.setFillColor(Color(255, 0, 0, min(128, Game::instance->powerTime * 3)));
    wnd->draw(rs);
}
void PowerDamageDecrease::onPowerTick(int tick)
{
    Game::instance->setGameSpeed(Game::instance->getGameSpeed() + 0.05f);
}
int PowerDamageDecrease::getCost()
{
    return 0;
}
string PowerDamageDecrease::getName()
{
    return "speed_increase";
}
