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

void GameplayObjectManager::registerCarType(std::string id, CarType* carType)
{
    DebugLogger::logDbg("Adding CarType: " + carType->getNamespacedId(id), "GameplayObjectManager");
    carTypes.add(carType->getNamespacedId(id), carType);
    carType->carId = id;
}

void GameplayObjectManager::registerPower(Game* game, std::string id, Power* obj)
{
    std::string s = obj->getNamespacedId(id);
    DebugLogger::logDbg("Adding power: " + s + " (" + obj->getName() + ")", "GameplayObjectManager");
    obj->id = s;

    if(!obj->isAntiPower())
    {
        if(powers.add(s, obj) < 0)
            return;
    }
    else
    {
        if(antiPowers.add(s, obj) < 0)
            return;
    }

    obj->onInit();
}

std::string operator+(std::string& str, int i)
{
    return str + std::to_string(i);
}
