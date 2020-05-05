#include "CarType.h"
#include "Game.h"

CarType::CarType(Car::TypeId id, string textureName) : carTexture(textureName), carId(id)
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

CarType* CarType::setRarities(std::initializer_list<int> list)
{
    auto arr = Game::instance->gpo.levels.arr();
    for(auto it = arr.begin(); it != arr.end(); it++)
    {
        // Reset rarities to default values, if not set
        carRarities[(LevelData::MapType)(it - arr.end())] = 1;
    }
	for(auto it = list.begin(); it != list.end(); it++)
    {
        // Set rarity to value specified in list.
        carRarities[(LevelData::MapType)(it - list.begin())] = *it;
    }

	return this;
}
CarType* CarType::setRarityFor(LevelData::MapType level, int rarity)
{
    carRarities[level] = rarity;
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

int CarType::getRarity(LevelData::MapType mapType)
{
	return carRarities[mapType];
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
