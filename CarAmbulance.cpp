#include "CarAmbulance.h"

CarAmbulance::CarAmbulance(float s, int l)
    : Car(s,l)
{
    this->typeId = AMBULANCE;
    this->maxHealth = 4;
    this->health = 3;
    this->textureName = "ambulance";
	this->animSize = 2;
}


