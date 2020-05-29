#ifndef COREEVENTHANDLERS_HPP
#define COREEVENTHANDLERS_HPP

#include "CoreLoader.hpp"

namespace CoreEventHandlers
{
    // GAME EVENTS

	// True if the car has to be spawned.
	CGAPI bool onCarSpawning(GameEvent& event, Game* game);

	// True if car should be damaged.
	CGAPI bool onCarDamaged(GameEvent& event, Game* game);

	// Not cancellable.
	CGAPI bool onLevelLoadingStart(GameEvent& event, Game* game);

	// Return false to cancel power start.
	CGAPI bool onPowerStarted(GameEvent& event, Game* game);

	// Return false to cancel power cooldown.
	CGAPI bool onPowerStopped(GameEvent& event, Game* game);
}

#endif //COREEVENTHANDLERS_HPP
