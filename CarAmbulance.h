#ifndef CARAMBULANCE_H
#define CARAMBULANCE_H

#include "Car.h"

class CarAmbulance : public Car
{
public:
    CarAmbulance()
    {
        this->typeId = AMBULANCE;
    }
    CarAmbulance(Car::TypeId, float, int);
    virtual ~CarAmbulance() {}

protected:

private:
};

#endif // CARAMBULANCE_H

