#include "CarLorry.h"

CarLorry::CarLorry(Car::TypeId id, float s, int l)
    : Car(id, s,l)
{
    this->setMaxHealth(3.f);
    this->textureName = "lorry";
    this->typeId = LORRY;
}
