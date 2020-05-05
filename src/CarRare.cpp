#include "CarRare.h"

CarRare::CarRare(Car::TypeId id, float s, int l)
    : Car(id, s,l)
{
}

void CarRare::onDamage(Game* game)
{
    game->addScore(2);
    makeDestroy(game->getDamageMultiplier());
}
