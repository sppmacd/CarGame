#ifndef CARBUS_H
#define CARBUS_H

#include "Car.h"

#include "CoreLoader.hpp"

class CarBus : public Car
{
public:
    CarBus()
    {
        this->typeId = CoreLoader::Cars::BUS;
    }
    CarBus(Car::TypeId, float, int);
    virtual ~CarBus() {}

protected:

private:
};

#endif // CARBUS_H

