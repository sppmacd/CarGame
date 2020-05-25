#pragma once

#include <SFML/Window.hpp>
#include "GameEvent.h"
#include <map>

class Game;

using namespace std;
using namespace sf;

// Event handlers for system events (e.g. mouse click). There are not cancelable.
typedef bool(*CGEventHandler)(Event,Game*);

// Event handlers for in-game events (e.g. car spawn). The handler should return true if event
// should be passed to next handler and should continue, and false if should be canceled
// (e.g. if CarSpawning event handler returns false, the car won't be spawned)
typedef bool(*GameEventHandler)(GameEvent&, Game*);

namespace EventHandlers
{
    // SYSTEM EVENTS

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
