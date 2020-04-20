#pragma once

#include <SFML/System.hpp>
#include "GameDisplay.h"
using namespace sf;

namespace LevelUtility
{
    // Returns lane ID by screen (game) position.
	int getLaneFromPos(Vector2f position);

	// Gets lane position (in game coordinates).
	Vector2f getLanePos(int lane);
}
