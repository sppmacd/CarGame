#pragma once

#include "Car.h"
#include "LevelData.h"

class CarType;

class GameEvent
{
public:

	enum Type
	{
		CarSpawning,        //Using CreateCarInstanceEvent  Cancelable=true
		CarSpawned,         //Using CarEvent                Cancelable=true
		CarDestroyed,       //Using CarEvent                Cancelable=true
		CarDeleted,         //Using CarEvent                Cancelable=true
		CarDamaged,         //Using CarEvent                Cancelable=true
		CarLeaved,          //Using CarEvent                Cancelable=true
		PreTick,            //Using none                    Cancelable=false
		PostTick,           //Using none                    Cancelable=false
        LevelLoadingStart,  //Using LevelEvent              Cancelable=false

		Count //Total count
	};
	Type type;

    struct CarEvent { Car* car; };
    struct CreateCarInstanceEvent { Car* carToCreate; CarType* type; };
    struct LevelEvent { LevelData* level; };

	union
	{
		CarEvent car;
		CreateCarInstanceEvent carSpawned;
		LevelEvent level;
	};

	GameEvent() {}
	~GameEvent() {}
};
