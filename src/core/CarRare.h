#ifndef CARRARE_H
#define CARRARE_H

#include "CoreLoader.hpp"

class CarRare : public Car
{
public:
    CarRare()
    {
        this->typeId = CoreLoader::Cars::RARE;
    }
    CarRare(Car::TypeId, float, int);

    // Called when the car is damaged by left mouse click.
    void onDamage(Game* game);
};

#endif // CARRARE_H
