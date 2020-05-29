#ifndef CORELOADER_HPP
#define CORELOADER_HPP

#include <cargame/API.hpp>

#include "CoreEventHandlers.hpp"

namespace CoreLoader
{
    void registerLevels(Game* game);
    void registerCars(Game* game);
    void registerPowers(Game* game);
    void registerEventHandlers(Game* game);
    void registerAbilities(PlayerAbilityManager* am);
}

extern "C"
{
    // Called when module is initialized in Game.
    CGAPI void cgGameInit();

    // Called when module is added to Game, before anything starts.
    CGAPI void cgLoad();

    // Called when Game is closed.
    CGAPI void cgCleanup();
}

#endif // CORELOADER_HPP
