#include "CarBomb.hpp"

CarBomb::CarBomb(float spd, int lane) : Car(Car::BOMB, spd, lane)
{
    setColor(Color::White);
    animSize = 10;
    frameLength = 3;
}
void CarBomb::onDestroy(Game* game)
{
    game->setGameOver();
}
void CarBomb::onLeave(Game*)
{
    //nothing
}
