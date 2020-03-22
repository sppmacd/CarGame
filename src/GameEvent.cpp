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

bool EventHandlers::onCarSpawning(GameEvent& event, Game *)
{
	Car::TypeId carId = event.carSpawned.type->carId;

	int color = rand() % 128 + 64;

	switch (carId)
	{
	case Car::NORMAL:
	{
		Car* car = new Car(Car::NORMAL, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(rand() % 128 + 64, rand() % 128 + 64, rand() % 128 + 64));
		break;
	}
	case Car::LORRY:
	{
		Car* car = new CarLorry(Car::LORRY, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(color, color, color));
		break;
	}
	case Car::RARE:
	{
		Car* car = new CarRare(Car::RARE, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(color, 0, 0));
		break;
	}
	case Car::BUS:
	{
		Car* car = new CarBus(Car::BUS, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(0, color, color));
		break;
	}
	case Car::AMBULANCE:
	{
		Car* car = new CarAmbulance(Car::AMBULANCE, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(255, 255, 255));
		break;
	}
	case Car::RALLY:
	{
		Car* car = new Car(Car::RALLY, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		break;
	}
	case Car::BOMB:
	{
		Car* car = new CarBomb(7.f, 1); // ALWAYS ON CENTER LANE
		event.carSpawned.carToCreate = car;
		break;
	}
	case Car::FIREMAN:
	{
		Car* car = new Car(Car::FIREMAN, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(200, 0, 0));
		if(rand() % 200 == 0)
            event.carSpawned.carToCreate->setColor(Color(0, 200, 0));
		break;
	}
	case Car::TANK:
	{
		Car* car = new Car(Car::TANK, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
        event.carSpawned.carToCreate->setColor(Color(92, 107, 85));
		break;
	}
	case Car::OLD:
	{
		Car* car = new Car(Car::OLD, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(35, 35, 35));
		break;
	}
	case Car::ARMORED:
	{
	    // check if armored car is already on the road
	    for(size_t s = 0; s < Game::instance->cars.size(); s++)
        {
            if(Game::instance->cars[s]->typeId == Car::ARMORED)
            {
                // replace armored car by rare car with the same color
                // TODO: "Armored Car" achievement
                Car* car = new CarRare(Car::RARE, 7.f, rand() % 3);
                event.carSpawned.carToCreate = car;
                event.carSpawned.carToCreate->setColor(Color(92, 107, 85));
                return true;
            }
        }
		Car* car = new Car(Car::ARMORED, 7.f, rand() % 3);
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(92, 107, 85));
		break;
	}
	case Car::TRAIN:
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

bool EventHandlers::onCarDamaged(GameEvent& event, Game* game)
{
    // handle PowerRamp
    Car* car = event.car.car;
    DebugLogger::logDbg("Car damaged: a=0x" + to_string((size_t)car) + ", pos=" + to_string(car->pos), "EventHandlers/onCarDamaged");
    return true;
}
