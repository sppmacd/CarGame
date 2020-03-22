#pragma once

#include "Car.h"
#include "LevelData.h"
;
class CarType;

class GameEvent
{
public:

	enum Type
	{
		CarSpawning,        //Using %carSpawned             Cancelable=true
		CarSpawned,         //Using %car                    Cancelable=true
		CarDestroyed,       //Using %car                    Cancelable=true
		CarDeleted,         //Using %car                    Cancelable=true
		CarDamaged,         //Using %car                    Cancelable=true
		CarLeaved,          //Using %car                    Cancelable=true
		PreTick,            //Using none                    Cancelable=false
		PostTick,           //Using none                    Cancelable=false
        LevelLoadingStart,  //Using %level                  Cancelable=false

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
