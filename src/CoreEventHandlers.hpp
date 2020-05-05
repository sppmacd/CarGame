#ifndef COREEVENTHANDLERS_HPP
#define COREEVENTHANDLERS_HPP

#include "CoreLoader.hpp"

namespace CoreEventHandlers
{
    // GAME EVENTS

	// True if the car has to be spawned.
	bool onCarSpawning(GameEvent& event, Game* game);

	// True if car should be damaged.
	bool onCarDamaged(GameEvent& event, Game* game);

	// Not cancellable.
	bool onLevelLoadingStart(GameEvent& event, Game* game);
}

#endif //COREEVENTHANDLERS_HPP
