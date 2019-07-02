#include "CarBus.h"

CarBus::CarBus(Car::TypeId id, float s, int l)
    : Car(id, s,l)
{
    this->setMaxHealth(5.f);
    this->textureName = "bus";
    this->typeId = BUS;
}
