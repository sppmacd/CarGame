#include "CarRare.h"

CarRare::CarRare(float s, int l)
    : Car(s,l)
{
    this->maxHealth = 2;
    this->health = 1;
    this->typeId = RARE;
}
