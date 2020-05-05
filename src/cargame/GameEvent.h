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
		// The car type is known but the car details no. Handle this 
		// event to setup car. This is necessary when adding Cars to game.
		CarSpawning,        //Using %carSpawned             Cancelable=true
		
		// The car finished generating and is about to place in map.
		CarSpawned,         //Using %car                    Cancelable=true
		
		// A car health reached 0 and it starts destroy animation.
		CarDestroyed,       //Using %car                    Cancelable=true
		
		// A car finished its destroy animation and is about to remove
		// from memory.
		CarDeleted,         //Using %car                    Cancelable=true
		
		// A car was left-clicked.
		CarDamaged,         //Using %car                    Cancelable=true
		
		// A car left the screen.
		CarLeaved,          //Using %car                    Cancelable=true
		
		// The damage is dealen to car. When called, user can
		// change the damage.
		DamageDealenCar,	//Using %carDamage				Cancelable=true
		
		// Power is started. The %tickTime argument can 
		// be used to manipulate target power time. Cancel the event
		// to disable power start.
		PowerStarted,		//Using %power					Cancelable=true
		
		// The power is ticked.
		PowerTick,			//Using %power					Cancelable=false
		
		// Power is stopped and cooldown is started. The %cooldownTime 
		// can be used to manipulate cooldown time. Cancel the event to
		// disable cooldown.
		PowerStopped,		//Using %power					Cancelable=true
		
		// The cooldown is ticked.
		PowerCooldownTick,	//Using %power					Cancelable=false
		
		// The cooldown is finished and the next power can be used.
		PowerCooldownStopped,//Using %power					Cancelable=false
		
		// The power is not used but is selected by player.
		PowerIdleTick,		//Using %power					Cancelable=false
		
		// The power is initialized after level is started.
		PowerInit,			//Using %powerInit				Cancelable=false
		
		// The power textures are loaded at startup.
		PowerTextureInit,	//Using %powerInit				Cancelable=false
		
		// Called before logic tick.
		PreTick,            //Using none                    Cancelable=false
		
		// Called after logic tick.
		PostTick,           //Using none                    Cancelable=false
		
		// Called before level loading starts.
        LevelLoadingStart,  //Using %level                  Cancelable=false
		
		Count //Total count
	};
	Type type;

	// Structure that concerns every Car event.
    struct CarEvent { Car* car; };

    // This structure is used when there are Car to create. Event handler should create Car dynamically
    // of specified %type and pass it to %carToCreate variable.
    struct CreateCarInstanceEvent { Car* carToCreate; CarType* type; };
	
	// Structure used when damage is dealed to car. You can modify the damage here. In core, used e.g.
	// in Ability.
	struct CarDamageEvent { Car* car; float& damage; };

    // It's every event that relates to level (e.g. event loading)
    struct LevelEvent { LevelData* level; };
	
	// Every event related to Power. In core used by some abilities.
	struct PowerEvent { Power* handle; int& tickTime; int& cooldownTime; };
	
	// Every event related to Power loading - don't need to pass tickTime and cooldownTime.
	struct PowerInitEvent { Power* handle; };

	union
	{
		CarEvent car;
		CreateCarInstanceEvent carSpawned;
		CarDamageEvent carDamage;
		LevelEvent level;
		PowerEvent power;
		PowerInitEvent powerInit;
	};

	GameEvent() {}
	~GameEvent() {}
};
