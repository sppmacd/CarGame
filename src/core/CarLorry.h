#ifndef CARLORRY_H
#define CARLORRY_H

#include "CoreLoader.hpp"

class CarLorry : public Car
{
public:
    CarLorry()
    {
        this->typeId = CoreLoader::Cars::LORRY;
    }
    CarLorry(Car::TypeId, float, int);
    virtual ~CarLorry() {}

protected:

private:
};

#endif // CARLORRY_H
