#include "CarLorry.h"

CarLorry::CarLorry(Car::TypeId id, float s, int l)
    : Car(id, s,l)
{
    this->maxHealth = 3;
    this->health = 2;
    this->textureName = "lorry";
    this->typeId = LORRY;
}
