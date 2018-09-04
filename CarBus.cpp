#include "CarBus.h"

CarBus::CarBus(float s, int l)
    : Car(s,l)
{
    this->maxHealth = 5;
    this->health = 4;
    this->textureName = "bus";
    this->typeId = BUS;
}
