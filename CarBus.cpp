#include "CarBus.h"

CarBus::CarBus(Car::TypeId id, float s, int l)
    : Car(id, s,l)
{
    this->maxHealth = 5;
    this->health = 4;
    this->textureName = "bus";
    this->typeId = BUS;
}
