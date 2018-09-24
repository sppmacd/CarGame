#pragma once

class Car;
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

	union
	{
		struct CarEvent { Car* car; } car;
		struct CreateCarInstanceEvent { Car* carToCreate; CarType* type; } carSpawned;
	};
};