#ifndef CARLORRY_H
#define CARLORRY_H

#include "Car.h"

class CarLorry : public Car
{
public:
    CarLorry()
    {
        this->typeId = LORRY;
    }
    CarLorry(Car::TypeId, float, int);
    virtual ~CarLorry() {}

protected:

private:
};

#endif // CARLORRY_H