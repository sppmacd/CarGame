#ifndef CARRARE_H
#define CARRARE_H

#include "Car.h"

class CarRare : public Car
{
public:
    CarRare()
    {
        this->typeId = RARE;
    }
    CarRare(float, int);

protected:

private:
};

#endif // CARRARE_H
