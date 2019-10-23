#include "EventHandler.h"
#include "Game.h"
#include "Car.h"
#include "CarLorry.h"
#include "CarRare.h"
#include "CarBus.h"
#include "CarBomb.hpp"
#include "CarAmbulance.h"
#include "CarTrain.hpp"
#include "GuiIngame.h"
#include <iostream>

EventsHandler* EventsHandler::instance;

bool EventHandlers::onClose(Event, Game* game)
{
	game->exit(0);
	return true;
}

bool EventHandlers::onMouseButtonReleased(Event event, Game* game)
{
    if(!game->paused())
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            game->wasReleased = true;
        }
        else if (event.mouseButton.button == sf::Mouse::Right)
        {
            if (game->powerCooldown <= 0 && game->powerTime <= 0 && game->powers[game->getCurrentPower()].getLevel() > 0)
                game->isPowerUsed = true;
        }
    }
    else
    {
        game->sound.playSound("click", 100.f);
    }
	return true;
}

bool EventHandlers::onMouseWheelScrolled(Event event, Game* game)
{
	game->wheelEvent(event.mouseWheelScroll);
	return true;
}

bool EventHandlers::onGUIKeyPressed(Event, Game*)
{
    /*
    if(game->isGuiLoaded())
        game->getCurrentGUI()->onKeyboard(event.key.code);
    */
    return true;
}

bool EventHandlers::onKeyPressed(Event event, Game* game)
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
		game->displayGui(new GuiIngame);
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
	return true;
}

//////////////////////////////////////////

EventsHandler::EventsHandler()
{
	instance = this;
}

void EventsHandler::registerGameEvent(GameEvent::Type event, GameEventHandler func)
{
	this->registry.insert(make_pair(event, func));
}
