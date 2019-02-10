#include "EventHandler.h"
#include "Game.h"
#include "Car.h"
#include "CarLorry.h"
#include "CarRare.h"
#include "CarBus.h"
#include "CarAmbulance.h"

EventsHandler* EventsHandler::instance;

bool EventHandlers::onClose(Event event, Game* game)
{
	game->exit(0);
	return true;
}

bool EventHandlers::onMouseButtonReleased(Event event, Game * game)
{
	if (event.mouseButton.button == sf::Mouse::Left)
	{
		game->wasReleased = true;
	}
	else if (event.mouseButton.button == sf::Mouse::Right)
	{
		if (game->powerCooldown <= 0 && game->powerTime <= 0 && game->powers[game->getCurrentPower()] > 0)
			game->isPowerUsed = true;
	}
	return true;
}

bool EventHandlers::onMouseWheelScrolled(Event event, Game * game)
{
	game->wheelEvent(event.mouseWheelScroll);
	return true;
}

bool EventHandlers::onGUIKeyPressed(Event event, Game* game)
{
    if(game->isGuiLoaded)
        game->displayedGui->onKeyboard(event.key.code);
    return true;
}

bool EventHandlers::onKeyPressed(Event event, Game * game)
{
	if (event.key.code == sf::Keyboard::Space)
	{
		if (game->tutorialStep == 6)
		{
			game->tutorialStep = 0;
			game->pause(false);
		}
	}
	else if (game->isGameOver() && event.key.code == sf::Keyboard::Return)
	{
		game->loadGame();
	}
	else if (event.key.code == sf::Keyboard::Escape && !game->isGameOver() && !game->paused())
	{
		cout << "main: Pausing game..." << endl;
		game->displayGui(0);
		game->pause(true);
	}
	else if (event.key.code == sf::Keyboard::F11)
	{
		game->toggleFullscreen();
	}
	else if (event.key.code == sf::Keyboard::F3 && event.key.shift)
	{
		game->debug = !game->debug;
	}
	else if (event.key.code == sf::Keyboard::Escape && game->displayedGui == 0) //close ingame GUI on Esc (0.0.5)
	{
		game->closeGui();
		game->pause(false);
	}
	return true;
}

bool EventHandlers::onCarSpawning(GameEvent& event, Game * game)
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
		Uint8 cc = rand() & 0b111;
		event.carSpawned.carToCreate->setColor(Color(cc & 0x4 ? color : 0, cc & 0x2 ? color : 0, cc & 0x1 ? color : 0));
		break;
	}
	case Car::BOMB:
	{
		Car* car = new Car(Car::BOMB, 7.f, 1); // ALWAYS ON CENTER LANE
		// TODO: create CarBomb
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color::White);

		// TODO: This will be moved to CarBomb class !!!
		event.carSpawned.carToCreate->animSize = 10;
		event.carSpawned.carToCreate->frameLength = 3;
		break;
	}
	case Car::FIREMAN:
	{
		Car* car = new Car(Car::FIREMAN, 7.f, rand() % 3);
		// TODO: create CarBomb
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(200, 0, 0));
		break;
	}
	case Car::TANK:
	{
		Car* car = new Car(Car::TANK, 7.f, rand() % 3);
		// TODO: create CarBomb
		event.carSpawned.carToCreate = car;
        event.carSpawned.carToCreate->setColor(Color(92, 107, 85));
		break;
	}
	case Car::OLD:
	{
		Car* car = new Car(Car::OLD, 7.f, rand() % 3);
		// TODO: create CarBomb
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(35, 35, 35));
		break;
	}
	case Car::ARMORED:
	{
		Car* car = new Car(Car::ARMORED, 7.f, rand() % 3);
		// TODO: create CarBomb
		event.carSpawned.carToCreate = car;
		event.carSpawned.carToCreate->setColor(Color(92, 107, 85));
		break;
	}
	default: return false;
	}
	return true;
}

EventsHandler::EventsHandler()
{
	instance = this;
}

void EventsHandler::registerGameEvent(GameEvent::Type event, GameEventHandler func)
{
	this->registry.insert(make_pair(event, func));
}
