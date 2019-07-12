#ifndef CARBUS_H
#define CARBUS_H

#include "Car.h"

class CarBus : public Car
{
public:
    CarBus()
    {
        this->typeId = BUS;
    }
    CarBus(Car::TypeId, float, int);
    virtual ~CarBus() {}

protected:

private:
};

#endif // CARBUS_H

