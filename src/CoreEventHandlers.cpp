#include "GameEvent.h"
#include "EventHandler.h"
#include "CarType.h"
#include "CarLorry.h"
#include "CarRare.h"
#include "CarBus.h"
#include "CarAmbulance.h"
#include "CarBomb.hpp"
#include "CarTrain.hpp"
#include "DebugLogger.hpp"

#include "CoreEventHandlers.hpp"

bool CoreEventHandlers::onLevelLoadingStart(GameEvent& event, Game* game)
{
    game->setDamageMultiplier(game->playerData.abilities.calculateValue(CoreLoader::Abilities::DAMAGE));
    return true;
}

bool CoreEventHandlers::onCarSpawning(GameEvent& event, Game*)
{
	int carId = event.carSpawned.type->carId;

	int color = rand() % 128 + 64;

	switch (carId)
	{
	case CoreLoader::Cars::NORMAL:
	{
		Car* car = new Car(CoreLoader::Cars::NORMAL, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(rand() % 128 + 64, rand() % 128 + 64, rand() % 128 + 64));
		break;
	}
	case CoreLoader::Cars::LORRY:
	{
		Car* car = new CarLorry(CoreLoader::Cars::LORRY, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(color, color, color));
		break;
	}
	case CoreLoader::Cars::RARE:
	{
		Car* car = new CarRare(CoreLoader::Cars::RARE, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(color, 0, 0));
		break;
	}
	case CoreLoader::Cars::BUS:
	{
		Car* car = new CarBus(CoreLoader::Cars::BUS, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(0, color, color));
		break;
	}
	case CoreLoader::Cars::AMBULANCE:
	{
		Car* car = new CarAmbulance(CoreLoader::Cars::AMBULANCE, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(255, 255, 255));
		break;
	}
	case CoreLoader::Cars::RALLY:
	{
		Car* car = new Car(CoreLoader::Cars::RALLY, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		break;
	}
	case CoreLoader::Cars::BOMB:
	{
		Car* car = new CarBomb(7.f, 1); // ALWAYS ON CENTER LANE
		event.carSpawned.carToCreate = car;
		break;
	}
	case CoreLoader::Cars::FIREMAN:
	{
		Car* car = new Car(CoreLoader::Cars::FIREMAN, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(200, 0, 0));
		if(rand() % 200 == 0)
            event.carSpawned.carToCreate->setColor(Color(0, 200, 0));
		break;
	}
	case CoreLoader::Cars::TANK:
	{
		Car* car = new Car(CoreLoader::Cars::TANK, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
        event.carSpawned.carToCreate->setColor(Color(92, 107, 85));
		break;
	}
	case CoreLoader::Cars::OLD:
	{
		Car* car = new Car(CoreLoader::Cars::OLD, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(35, 35, 35));
		break;
	}
	case CoreLoader::Cars::ARMORED:
	{
	    // check if armored car is already on the road
	    for(size_t s = 0; s < Game::instance->cars.size(); s++)
        {
            if(Game::instance->cars[s]->typeId == CoreLoader::Cars::ARMORED)
            {
                // replace armored car by rare car with the same color
                // TODO: "Armored Car" achievement
                Car* car = new CarRare(CoreLoader::Cars::RARE, 7.f, rand() % 3);
                event.carSpawned.carToCreate = car;
                event.carSpawned.carToCreate->setColor(Color(92, 107, 85));
                return true;
            }
        }
		Car* car = new Car(CoreLoader::Cars::ARMORED, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(92, 107, 85));
		break;
	}
	case CoreLoader::Cars::TRAIN:
    {
        Car* car = new CarTrain(7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(193, 205, 155));
		break;
    }
	default:
        return false;
	}
	return true;
}

bool CoreEventHandlers::onCarDamaged(GameEvent& event, Game* game)
{
    // handle PowerRamp
    Car* car = event.car.car;
    DebugLogger::logDbg("Car damaged: a=" + to_string((size_t)car) + ", pos=" + to_string(car->pos) + ", type=" + to_string(car->typeId), "EventHandlers", "EVENT");
    return true;
}
