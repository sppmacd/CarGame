#pragma once

class Car;

class GameEvent
{
public:

	enum Type
	{
		CarSpawning, //Using UnknownCarEvent
		CarSpawned, //Using CarEvent
		CarDestroyed, //Using CarEvent
		CarDeleted, //Using CarEvent
		CarDamaged, //Using CarEvent
		Count //Total count
	};
	Type type;

	union Events
	{
		struct CarEvent { Car* car; } car;
		struct UnknownCarEvent { Car* car; } unknownCar;
	};
};