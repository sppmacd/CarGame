#pragma once

#include <SFML/Window.hpp>
#include "GameEvent.h"
#include <map>

class Game;

using namespace std;
using namespace sf;

typedef bool(*CGEventHandler)(Event,Game*);
typedef bool(*GameEventHandler)(GameEvent&, Game*);

namespace EventHandlers
{
	// Called when the window close is requested.
	bool onClose(Event event, Game* game);
	
	// Called when the mouse button is released.
	bool onMouseButtonReleased(Event event, Game* game);
	
	// Called when the mouse wheel is scrolled.
	bool onMouseWheelScrolled(Event event, Game* game);
	
	// Called when the keyboard key is pressed.
	bool onKeyPressed(Event event, Game* game);
	
	// Not used.
	SFML_DEPRECATED bool onGUIKeyPressed(Event event, Game* game);

	// True if the car has to be spawned.
	bool onCarSpawning(GameEvent& event, Game* game);

	// True if car should be damaged.
	bool onCarDamaged(GameEvent& event, Game* game);
}

// Class holding game event handlers.
class EventsHandler
{
public:
	EventsHandler();
	
	// The singleton.
	static EventsHandler* instance;
	
	// Game event registry. Types defined in <GameEvent.h>.
	multimap<GameEvent::Type, GameEventHandler> registry;
	
	// Register new GameEvent. Types defined in <GameEvent.h>.
	void registerGameEvent(GameEvent::Type event, GameEventHandler func);
};
