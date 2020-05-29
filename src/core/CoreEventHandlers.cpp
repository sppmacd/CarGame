#include "CoreLoader.hpp"
#include "CarRare.h"
#include "CarAmbulance.h"
#include "CarBomb.hpp"
#include "CarTrain.hpp"
#include "CoreEventHandlers.hpp"

bool CoreEventHandlers::onLevelLoadingStart(GameEvent& event, Game* game)
{
    game->setDamageMultiplier(game->playerData.abilities.calculateValue("damage"));
    return true;
}

bool CoreEventHandlers::onCarSpawning(GameEvent& event, Game*)
{
	std::string carId = event.carSpawned.type->carId.getObjectId();

	if(event.carSpawned.type->carId.getModule() != "cgcore")
        return false;

	int color = rand() % 128 + 64;

	if(carId == "default")
	{
		Car* car = new Car("default", 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(rand() % 128 + 64, rand() % 128 + 64, rand() % 128 + 64));
	}
	else if(carId == "lorry")
	{
		Car* car = new Car("lorry", 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(color, color, color));
	}
	else if(carId == "rare")
	{
		Car* car = new CarRare(7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(color, 0, 0));
	}
	else if(carId == "bus")
	{
		Car* car = new Car("bus", 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(0, color, color));
	}
	else if(carId == "ambulance")
	{
		Car* car = new CarAmbulance(7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(255, 255, 255));
	}
	else if(carId == "rally")
	{
		Car* car = new Car("rally", 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
	}
	else if(carId == "bomb")
	{
		Car* car = new CarBomb(7.f, 1); // ALWAYS ON CENTER LANE
		event.carSpawned.carToCreate = car;
	}
	else if(carId == "fireman")
	{
		Car* car = new Car("fireman", 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(200, 0, 0));
		if(rand() % 200 == 0)
            event.carSpawned.carToCreate->setColor(Color(0, 200, 0));
	}
	else if(carId == "tank")
	{
		Car* car = new Car("tank", 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
        event.carSpawned.carToCreate->setColor(Color(92, 107, 85));
	}
	else if(carId == "old")
	{
		Car* car = new Car("old", 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(35, 35, 35));
	}
	else if(carId == "armored")
	{
	    // check if armored car is already on the road
	    for(size_t s = 0; s < Game::instance->cars.size(); s++)
        {
            if(Game::instance->cars[s]->typeId == "armored")
            {
                // replace armored car by rare car with the same color
                // TODO: "Armored Car" achievement
                Car* car = new CarRare(7.f, rand() % 3);
                event.carSpawned.carToCreate = car;
                event.carSpawned.carToCreate->setColor(Color(92, 107, 85));
                return true;
            }
        }
		Car* car = new Car("armored", 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(92, 107, 85));
	}
	else if(carId == "train")
    {
        Car* car = new CarTrain(7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(193, 205, 155));
    }
	else
        return false;

	return true;
}

bool CoreEventHandlers::onCarDamaged(GameEvent& event, Game* game)
{
    // handle PowerRamp
    Car* car = event.car.car;
    DebugLogger::logDbg("Car damaged: a=" + to_string((size_t)car) + ", pos=" + to_string(car->pos) + ", type=" + car->typeId.toString(), "EventHandlers", "EVENT");
    return true;
}

bool CoreEventHandlers::onPowerStarted(GameEvent& event, Game* game)
{
    Power* power = event.power.handle;
    if(!power->isAntiPower())
    {
        (*event.power.tickTime) *= game->playerData.abilities.calculateValue("power_time");
    }
    return true;
}

bool CoreEventHandlers::onPowerStopped(GameEvent& event, Game* game)
{
    Power* power = event.power.handle;
    if(!power->isAntiPower())
    {
        (*event.power.cooldownTime) /= game->playerData.abilities.calculateValue("power_cooldown_time");
    }
    return true;
}
