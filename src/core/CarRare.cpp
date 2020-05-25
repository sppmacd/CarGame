#include "CarRare.h"

CarRare::CarRare(float s, int l)
    : Car("rare", s,l)
{
}

void CarRare::onDamage(Game* game)
{
    game->addScore(2);
    makeDestroy(game->getDamageMultiplier());
}
