#include "GameplayObjectManager.hpp"
#include "Game.h"
#include "DebugLogger.hpp"
#include "Power.h"

void GameplayObjectManager::clear()
{
    carTypes.clear();
    levels.clear();
    powers.clear();
    user.clear();
}

void GameplayObjectManager::registerPower(Game* game, int id, Power* obj)
{
    int rid = id;

    // Automatically determine ID for special values.
    if(id == POWER_REGULAR_ID_START)
    {
        rid = game->biggestPlayerPowerID + 1;
    }
    else if(id == POWER_ANTI_ID_START)
    {
        rid = game->biggestGenericPowerID + 1;
    }

    // Don't add power if it exceedes power limit
    if(id == POWER_REGULAR_ID_START && rid > POWER_ANTI_ID_START)
    {
        DebugLogger::logDbg("Couldn't add regular power, power limit exceeded: " + obj->getName(), "GameplayObjectManager", "ERROR");
        return;
    }

    DebugLogger::logDbg("Adding power: " + std::to_string(id) + " (" + obj->getName() + ")", "GameplayObjectManager");
    obj->id = rid;

    if(!powers.add(rid, obj))
        return;

    if(id <= 100)
    {
        if(id > game->biggestPlayerPowerID)
            game->biggestPlayerPowerID = id;
    }
    else
    {
        if(id > game->biggestGenericPowerID)
            game->biggestGenericPowerID = id;
    }
    obj->onInit();
}

std::string operator+(std::string& str, int i)
{
    return str + std::to_string(i);
}
