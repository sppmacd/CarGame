#include "PowerDamageDecrease.hpp"
#include "CoreLoader.hpp"

PowerDamageDecrease::PowerDamageDecrease()
    : Power("cgcore")
{
    setMaxTime(500);
    cooldownTime = 0;
}
void PowerDamageDecrease::drawPower(RenderWindow* wnd)
{
    RectangleShape rs(Vector2f(1920.f, 1080.f));
    rs.setFillColor(Color(152, 39, 204, min(128, Game::instance->powerTime * 3)));
    wnd->draw(rs);
}
bool PowerDamageDecrease::onPowerStart()
{
    Game::instance->setDamageMultiplier(Game::instance->getDamageMultiplier() / 20.f);
    return true;
}
void PowerDamageDecrease::onPowerStop()
{
    Game::instance->setDamageMultiplier(Game::instance->getDamageMultiplier() * 20.f);
}
int PowerDamageDecrease::getCost()
{
    return 0;
}
string PowerDamageDecrease::getName()
{
    return "damage_decrease";
}
