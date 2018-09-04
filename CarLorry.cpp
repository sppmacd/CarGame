#include "CarLorry.h"

CarLorry::CarLorry(float s, int l)
    : Car(s,l)
{
    this->maxHealth = 3;
    this->health = 2;
    this->textureName = "lorry";
    this->typeId = LORRY;
}
