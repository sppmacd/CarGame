#include "PlayerAbilityManager.hpp"
#include "Game.h"

PlayerAbilityManager::~PlayerAbilityManager()
{
    for(pair<const ModuleIdentifier, AbilityBase*>& _pair: abilities)
    {
        delete _pair.second;
    }
}

bool PlayerAbilityManager::upgradeAbility(Game* game, ModuleIdentifier ability)
{
    auto it = abilityLevels.find(ability);
    if(it == abilityLevels.end())
        return false; //ability not found

    unsigned int cost = calculateCost(ability);
    if(game->playerData.playerCoins < cost)
        return false; //not enough coins

    game->removeCoins(cost);
    it->second++; //change ability level
    return true; //success!!
}

unsigned int PlayerAbilityManager::getAbilityLevel(ModuleIdentifier ability)
{
    auto it = abilityLevels.find(ability);
    if(it == abilityLevels.end())
        return 0; //ability not found
    return it->second;
}

double PlayerAbilityManager::calculateValue(ModuleIdentifier ability, int level)
{
    if(level == -1)
        level = getAbilityLevel(ability);

    auto eq = abilities.find(ability);
    if(eq == abilities.end())
        return 0.0; //ability not found

    return eq->second->calculateValue(level);
}

unsigned int PlayerAbilityManager::calculateCost(ModuleIdentifier ability, int level)
{
    if(level == -1)
        level = getAbilityLevel(ability) + 1;

    auto eq = abilities.find(ability);
    if(eq == abilities.end())
        return 0; //ability not found

    return eq->second->calculateCost(level);
}

void PlayerAbilityManager::write(HMDataMap& _map)
{
    for(pair<const ModuleIdentifier, AbilityBase*>& _pair: abilities)
    {
        _map.setNumberKey(_pair.second->getName(), getAbilityLevel(_pair.first), "ability");
    }
}

void PlayerAbilityManager::read(HMDataMap& _map)
{
    for(pair<const ModuleIdentifier, AbilityBase*>& _pair: abilities)
    {
        abilityLevels[_pair.first] = _map.getNumberKey(_pair.second->getName(), "ability", 0.0);
    }
}

void PlayerAbilityManager::clear()
{
    abilityLevels.clear();
}

void PlayerAbilityManager::registerAbility(ModuleIdentifier id, AbilityBase* base)
{
    abilities.insert(make_pair(id, base));
}

void PlayerAbilityManager::init()
{

}
