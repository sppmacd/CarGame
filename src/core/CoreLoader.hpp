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
    namespace Cars
    {
        // Default car IDs.
        extern int NORMAL,
        LORRY,
        RARE,
        BUS,
        AMBULANCE,
        RALLY,
        BOMB,
        FIREMAN,
        TANK,
        OLD,
        ARMORED,
        TRAIN;
    }
    namespace MapIds
    {
        // Map type variables - ID is automatically generated.
        // todo: move to core dll
        extern LevelData::MapType COUNTRYSIDE;
        extern LevelData::MapType DESERT;
        extern LevelData::MapType FOREST;
        extern LevelData::MapType ICE;
        extern LevelData::MapType MOUNTAINS;
        extern LevelData::MapType SEA;
        extern LevelData::MapType BEACH;
        extern LevelData::MapType SWAMPLAND;
        extern LevelData::MapType CITY;
        extern LevelData::MapType MOTORWAY;
        extern LevelData::MapType FOREST_TRAILS;
        extern LevelData::MapType COUNT;
    }

    void registerLevels(Game* game);
    void registerCars(Game* game);
    void registerPowers(Game* game);
    void registerEventHandlers(Game* game);
    void registerAbilities(PlayerAbilityManager* am);
}

// Called when module is initialized in Game.
CGAPI void cgGameInit(Game* game);

// Called when module is added to Game, before anything starts.
CGAPI void cgLoad(GameLoader* loader);

#endif // CORELOADER_HPP
