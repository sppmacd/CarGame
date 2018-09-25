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

bool EventHandlers::onCarSpawning(GameEvent event, Game * game)
{
	Car::TypeId carId = event.carSpawned.type->carId;

	int color = rand() % 64 + 128;

	switch (carId)
	{
	case Car::NORMAL: 
		event.carSpawned.carToCreate = &Car(Car::NORMAL, game->getGameSpeed(), rand() % 3);
		event.carSpawned.carToCreate->setColor(Color(rand() % 64 + 128, rand() % 64 + 128, rand() % 64 + 128));
		break;
	case Car::LORRY:
		event.carSpawned.carToCreate = &CarLorry(Car::LORRY, game->getGameSpeed(), rand() % 3);
		event.carSpawned.carToCreate->setColor(Color(color, color, color));
		break;
	case Car::RARE:
		event.carSpawned.carToCreate = &CarRare(Car::RARE, game->getGameSpeed(), rand() % 3);
		event.carSpawned.carToCreate->setColor(Color(color, 0, 0));
		break;
	case Car::BUS:
		event.carSpawned.carToCreate = &CarBus(Car::BUS, game->getGameSpeed(), rand() % 3);
		event.carSpawned.carToCreate->setColor(Color(0, color, color));
		break;
	case Car::AMBULANCE:
		event.carSpawned.carToCreate = &CarAmbulance(Car::AMBULANCE, game->getGameSpeed(), rand() % 3);
		event.carSpawned.carToCreate->setColor(Color(color, color, color));
		break;
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