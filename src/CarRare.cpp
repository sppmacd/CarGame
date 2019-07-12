#include "CarRare.h"

CarRare::CarRare(Car::TypeId id, float s, int l)
    : Car(id, s,l)
{
    this->setMaxHealth(2.f);
    this->typeId = RARE;
}
