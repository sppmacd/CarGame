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
    DebugLogger::logDbg("Adding power: " + std::to_string(id) + " (" + obj->getName() + ")", "GameplayObjectManager");
    obj->id = id;

    if(!powers.add(id, obj))
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
