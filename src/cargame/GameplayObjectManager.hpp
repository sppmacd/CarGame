#ifndef GAMEPLAYOBJECTMANAGER_HPP
#define GAMEPLAYOBJECTMANAGER_HPP

#include <string>
#include "GPORegistry.hpp"

#include "Car.h"
#include "CarType.h"
#include "LevelData.h"
#include "Power.h"

#define POWER_REGULAR_ID_START 0
#define POWER_ANTI_ID_START 100

class Game;

class GameplayObjectManager
{
public:
    GPORegistry<Car::TypeId, CarType> carTypes;
    GPORegistry<LevelData::MapType, LevelData> levels;
    GPORegistry<ModuleIdentifier, Power> powers;
    GPORegistry<ModuleIdentifier, Power> antiPowers;
    GPORegistry<ModuleIdentifier, void> user;

    // Clear all registries.
    void clear();

    // Register power. It needs some attention because there
    // are Powers and Anti-Powers and Game needs to set some
    // variables basing on ID.
    void registerPower(Game* game, std::string id, Power* obj);

    // Register car. It assigns ID to CarType after registering.
    void registerCarType(std::string id, CarType* carType);
};

// Add int to std::string.
std::string operator+(std::string&, int);

#endif // GAMEPLAYOBJECTMANAGER_HPP

