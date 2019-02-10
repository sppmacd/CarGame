#pragma once

#include "Car.h"
class CarType;

class GameEvent
{
public:

	enum Type
	{
		CarSpawning, //Using CreateCarInstanceEvent
		CarSpawned, //Using CarEvent
		CarDestroyed, //Using CarEvent
		CarDeleted, //Using CarEvent
		CarDamaged, //Using CarEvent
		Count //Total count
	};
	Type type;

    struct CarEvent { Car* car; };
    struct CreateCarInstanceEvent { Car* carToCreate; CarType* type; };

	union
	{
		CarEvent car;
		CreateCarInstanceEvent carSpawned;
	};

	GameEvent() {}
	~GameEvent() {}
};
