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
    rs.setFillColor(Color(152, 39, 204, min(128, Game::instance->powerTime * 3)));
    wnd->draw(rs);
}
bool PowerDamageDecrease::onPowerStart()
{
    Game::instance->setDamageMultiplier(Game::instance->getDamageMultiplier() / 2.f);
    return true;
}
void PowerDamageDecrease::onPowerStop()
{
    Game::instance->setDamageMultiplier(Game::instance->getDamageMultiplier() * 2.f);
}
int PowerDamageDecrease::getCost()
{
    return 0;
}
string PowerDamageDecrease::getName()
{
    return "damage_decrease";
}
