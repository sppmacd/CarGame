#ifndef CORELOADER_HPP
#define CORELOADER_HPP

#include <cargame/API.hpp>

#include "CoreEventHandlers.hpp"

namespace CoreLoader
{
    namespace Abilities
    {
        enum
        {
            POWER_COOLDOWN_TIME     = 0,
            POWER_TIME              = 1,
            DAMAGE                  = 2
        };
    }

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
    CGAPI void cgLoad(GameLoader* loader);

    // Called when Game is closed.
    CGAPI void cgCleanup();
}

#endif // CORELOADER_HPP
