#include "CarAmbulance.h"

CarAmbulance::CarAmbulance(float s, int l)
    : Car("ambulance",s,l)
{
	this->animSize = 2;
	this->frameLength = 8;
}
