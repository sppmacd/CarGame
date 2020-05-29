#pragma once

#include <HackerMan/Util/Main.hpp>
#include <vector>
#include <map>
#include "AbilityBase.hpp"
#include <cargame/ModuleIdentifier.hpp>

class Game;

// Class to manage player abilities (upgraded in Shop)
class PlayerAbilityManager
{
public:
    virtual ~PlayerAbilityManager();

    // Upgrades ability and removes coins. Returns true if player has PAID for ability.
    bool upgradeAbility(Game* game, ModuleIdentifier ability);

    // Get level of ability.
    unsigned int getAbilityLevel(ModuleIdentifier ability);

    // Calculate ability value from level. Set no level to use current level. Equation: baseValue*level^2
    double calculateValue(ModuleIdentifier ability, int level = -1);

    // Calculate cost of specified ability level. Set no level to use next level. Equation: baseCost*level^2 (same as powers)
    unsigned int calculateCost(ModuleIdentifier ability, int level = -1);

    // Write ability levels to profile.
    void write(HMDataMap& map);

    // Read ability levels from profile.
    void read(HMDataMap& map);

    // Clear ability map (set all abilities to 0). Used before reading player profile.
    void clear();

    // Register ability - add it to map. It will be now loaded from profile. Ability must be created dynamically - it's automatically deleted.
    void registerAbility(ModuleIdentifier id, AbilityBase* base);

    // Register all default abilities.
    void init();

private:
    friend class GuiShop;

    std::map<ModuleIdentifier, AbilityBase*> abilities; //Registered abilities.
    std::map<ModuleIdentifier, unsigned int> abilityLevels; //Ability values.
};
