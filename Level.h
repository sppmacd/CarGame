#pragma once

#include <SFML/System.hpp>
using namespace sf;

namespace LevelUtility
{
	int getLaneFromPos(Vector2f position)
	{
		float windowMiddle = GameDisplay::instance->getRenderWnd()->getSize().y / 2;
		if (position.y <= windowMiddle - 50)
			return 0;
		else if (position.y > windowMiddle - 50 && pos.y < windowMiddle + 50)
			return 1;
		else if (position.y >= windowMiddle + 50)
			return 2;
	}

	Vector2f getLanePos(int lane)
	{
		//	   |--------------------window center---------------------|
		return (GameDisplay::instance->getRenderWnd()->getSize().y / 2) + (lane-1) * 50;
	}
}