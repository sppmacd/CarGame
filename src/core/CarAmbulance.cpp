#include "CarAmbulance.h"

CarAmbulance::CarAmbulance(Car::TypeId id, float s, int l)
    : Car(id,s,l)
{
	this->animSize = 2;
	this->frameLength = 8;
}

