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

	// Structure that concerns every Car event.
    struct CarEvent { Car* car; };

    // This structure is used when there are Car to create. Event handler should create Car dynamically
    // of specified %type and pass it to %carToCreate variable.
    struct CreateCarInstanceEvent { Car* carToCreate; CarType* type; };

    // It's every event that relates to level (e.g. event loading)
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
