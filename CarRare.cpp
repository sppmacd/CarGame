#include "CarRare.h"

CarRare::CarRare(Car::TypeId id, float s, int l)
    : Car(id, s,l)
{
    this->maxHealth = 2;
    this->health = 1;
    this->typeId = RARE;
}
