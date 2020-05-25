#ifndef CARRARE_H
#define CARRARE_H

#include "CoreLoader.hpp"

class CarRare : public Car
{
public:
    CarRare(float, int);

    // Called when the car is damaged by left mouse click.
    void onDamage(Game* game);
};

#endif // CARRARE_H
