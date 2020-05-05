#ifndef CARAMBULANCE_H
#define CARAMBULANCE_H

#include "Car.h"
#include "CoreLoader.hpp"

class CarAmbulance : public Car
{
public:
    CarAmbulance()
    {
        this->typeId = CoreLoader::Cars::AMBULANCE;
    }
    CarAmbulance(Car::TypeId, float, int);
    virtual ~CarAmbulance() {}

protected:

private:
};

#endif // CARAMBULANCE_H


