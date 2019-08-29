#include "PowerPointBoost.hpp"
#include "Game.h"

PowerPointBoost::PowerPointBoost(): Power()
{
    setMaxTime(1000);
}
void PowerPointBoost::drawPower(RenderWindow* wnd)
{
    RectangleShape rs(Vector2f(wnd->getSize()));
    rs.setFillColor(Color(183, 24, 24, (Game::instance->powerTime / 1200.f) * 64.f));
    wnd->draw(rs);
}
void PowerPointBoost::onPowerTick(int tick)
{
    if(tick % 10 == 0)
        Game::instance->addScore(2 * currentLevel);
    Game::instance->setGameSpeed(Game::instance->getGameSpeed() + 0.005f);
}
int PowerPointBoost::getCost()
{
    return 2000;
}
string PowerPointBoost::getName()
{
    return "point_boost";
}
