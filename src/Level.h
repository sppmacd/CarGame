#pragma once

#include <SFML/System.hpp>
#include "GameDisplay.h"
using namespace sf;

namespace LevelUtility
{
	int getLaneFromPos(Vector2f position);
	Vector2f getLanePos(int lane);
}
