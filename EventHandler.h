#pragma once

#include <SFML/Window.hpp>
#include "GameEvent.h"
#include <map>

class Game;

using namespace std;
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

class EventsHandler
{
public:
	EventsHandler();
	static EventsHandler* instance;
	map<GameEvent::Type, GameEventHandler> registry;
	void registerGameEvent(GameEvent::Type event, GameEventHandler func);
};