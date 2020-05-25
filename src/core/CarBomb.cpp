#include "CarBomb.hpp"

CarBomb::CarBomb(float spd, int lane) : Car("bomb", spd, lane)
{
    setColor(Color::White);
    animSize = 10;
    frameLength = 3;
}
void CarBomb::onDamage(Game* game)
{
    int b = game->biggestGenericPowerID;
    makeDestroy(1e38f); //make very big damage to ensure car is destroyed.
    if(b - 100 == 0)
    {
        DebugLogger::log("No anti-power found!", "CarBomb", "INFO");
        return;
    }
    Power* power = game->gpo.powers.findById(rand() % (game->biggestGenericPowerID - 100) + 101);
    if(power)
    {
        game->setCurrentPower(power);
    }
}
void CarBomb::onLeave(Game*)
{
    //nothing
}
