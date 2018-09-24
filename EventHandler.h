#pragma once

#include <SFML/Window.hpp>
#include "GameEvent.hpp"

class Game;

using namespace sf;

typedef bool(*EventHandler)(Event,Game*);
typedef bool(*GameEventHandler)(GameEvent, Game*);

namespace EventHandlers
{
	bool onClose(Event event, Game* game);
	bool onMouseButtonReleased(Event event, Game* game);
	bool onMouseWheelScrolled(Event event, Game* game);
	bool onKeyPressed(Event event, Game* game);
	
	// True if the car has to be spawned.
	bool onCarSpawning(GameEvent event, Game* game);
}

class EventHandler
{
public:
	static EventHandler* instance;
	map<GameEvent::Type, GameEventHandler> registry;
	void registerGameEvent(GameEvent::Type event, GameEventHandler func);
	GameEventHandler getHandlersByEvent(GameEvent::Type event);
};