#pragma once

#include <SFML/System.hpp>
#include "GameDisplay.h"
using namespace sf;

namespace LevelUtility
{
	int getLaneFromPos(Vector2f position)
	{
		float windowMiddle = GameDisplay::instance->getRenderWnd()->getSize().y / 2;
		if (position.y <= windowMiddle - 50)
			return 0;
		else if (position.y > windowMiddle - 50 && position.y < windowMiddle + 50)
			return 1;
		else if (position.y >= windowMiddle + 50)
			return 2;
	}

	Vector2f getLanePos(int lane)
	{
		//	   |--------------------window center---------------------|
		return Vector2f(0.f,(GameDisplay::instance->getRenderWnd()->getSize().y / 2) + (lane-1) * 100);
	}
}