#include "CarBomb.hpp"

CarBomb::CarBomb(float spd, int lane) : Car(Car::BOMB, spd, lane)
{
    setColor(Color::White);
    animSize = 10;
    frameLength = 3;
}
void CarBomb::onDamage(Game* game)
{
    auto power = game->powerRegistry.find(rand() % (game->biggestGenericPowerID - 100) + 101);
    if(power != game->powerRegistry.end())
    {
        game->setCurrentPower(power->second);
    }
    makeDestroy(1e38f); //make infinite damage to ensure car is destroyed.
}
void CarBomb::onLeave(Game*)
{
    //nothing
}
