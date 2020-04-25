#include "CarBomb.hpp"

CarBomb::CarBomb(float spd, int lane) : Car(Car::BOMB, spd, lane)
{
    setColor(Color::White);
    animSize = 10;
    frameLength = 3;
}
void CarBomb::onDamage(Game* game)
{
    Power* power = game->gpo.powers.findById(rand() % (game->biggestGenericPowerID - 100) + 101);
    if(power)
    {
        game->setCurrentPower(power);
    }
    makeDestroy(1e38f); //make very big damage to ensure car is destroyed.
}
void CarBomb::onLeave(Game*)
{
    //nothing
}
