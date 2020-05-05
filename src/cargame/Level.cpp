#include "Level.h"

namespace LevelUtility
{
	int getLaneFromPos(Vector2f position)
	{
		float windowMiddle = 540.f;
		if (position.y <= windowMiddle - 50)
			return 0;
		else if (position.y > windowMiddle - 50 && position.y < windowMiddle + 50)
			return 1;
		else if (position.y >= windowMiddle + 50)
			return 2;
        return -1;
	}

	Vector2f getLanePos(int lane)
	{
		//	            |-window center-|
		return Vector2f(0.f, 540.f + (lane-1) * 100);
	}
}
