#include "CarType.h"
#include "Game.h"

CarType::CarType(string textureName)
    : GameplayObject("cgcore") //todo: replace core with modulemanager
    , carTexture(textureName)
    , defaultRarity(2)
    , maxHealth(1)
{
}

bool CarType::operator==(CarType & type2)
{
	return type2.carId == this->carId;
}

bool CarType::operator==(Car::TypeId id)
{
	return id == this->carId;
}

bool CarType::operator!=(Car::TypeId id)
{
	return !(*this == id);
}

bool CarType::operator!=(CarType & type2)
{
	return !(*this == type2);
}

CarType* CarType::setDefaultRarity(int rarity)
{
    defaultRarity = rarity;
    return this;
}

CarType* CarType::setRarityFor(ModuleIdentifier modId, int rarity)
{
    carRarities[modId] = rarity;
    return this;
}

CarType* CarType::setMaxHealth(float max)
{
	this->maxHealth = max;
	return this;
}

string CarType::getTextureName()
{
	return carTexture;
}

float CarType::getMaxHealth()
{
	return maxHealth;
}

int CarType::getRarity(ModuleIdentifier modId)
{
	auto it = carRarities.find(modId.toString());
	if(it == carRarities.end())
        return defaultRarity;
    return it->second;
}

/*
    Maps:
    COUNTRYSIDE,
    DESERT,
    FOREST,
    ICE,
    MOUNTAINS,
    BEACH,
    CITY,
    SWAMPLAND,
    SEA,
    MOTORWAY,
    FOREST_TRAILS,
*/
