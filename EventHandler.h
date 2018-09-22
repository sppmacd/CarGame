#pragma once

#include <SFML/Window.hpp>

class Game;

using namespace sf;

typedef bool(*EventHandler)(Event,Game*);

namespace EventHandlers
{
	bool onClose(Event event, Game* game);
	bool onMouseButtonReleased(Event event, Game* game);
	bool onMouseWheelScrolled(Event event, Game* game);
	bool onKeyPressed(Event event, Game* game);
}