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

CarType& CarType::setRarities(std::initializer_list<int> list)
{
    for(size_t s = 0; s < Game::instance->levelRegistry.size(); s++)
    {
        // Reset rarities to default values, if not set
        carRarities[(LevelData::MapType)s] = 1;
    }
	for(auto it = list.begin(); it != list.end(); it++)
    {
        // Set rarity to value specified in list.
        carRarities[(LevelData::MapType)(it - list.begin())] = *it;
    }

	return *this;
}
CarType& CarType::setRarityFor(LevelData::MapType level, int rarity)
{
    carRarities[level] = rarity;
    return *this;
}

CarType & CarType::setMaxHealth(float max)
{
	this->maxHealth = max;
	return *this;
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

void CarType::init()
{
	Game* game = Game::instance;
	game->registerCarType(CarType(Car::NORMAL, "default").setRarities(      {2,2,2,2,2,2,2,2,2,2,2}             ).setMaxHealth(1));
	game->registerCarType(CarType(Car::LORRY, "lorry").setRarities(         {3,3,3,15,13,5,2,5,10,3,15}         ).setMaxHealth(3));
	game->registerCarType(CarType(Car::RARE, "default").setRarities(        {5,3,3,4,7,8,10,5,5,3,5}            ).setMaxHealth(2));
	game->registerCarType(CarType(Car::BUS, "bus").setRarities(             {10,3,14,8,3,3,5,3,5,8,3}           ).setMaxHealth(5));
	game->registerCarType(CarType(Car::AMBULANCE, "ambulance").setRarities( {10,3,10,15,20,5,3,10,3,3,6}        ).setMaxHealth(4));
	game->registerCarType(CarType(Car::RALLY, "rally").setRarities(         {4,20,5,25,10,20,10,20,20,5,20}     ).setMaxHealth(2));
	game->registerCarType(CarType(Car::BOMB, "bomb").setRarities(           {1,15,15,15,15,15,15,15,15,15,15}  ).setMaxHealth(1));
	game->registerCarType(CarType(Car::FIREMAN, "fireman").setRarities(     {5,7,3,10,8,12,4,10,5,5,12}         ).setMaxHealth(5));
	game->registerCarType(CarType(Car::TANK, "tank").setRarities(           {10,7,15,20,25,10,20,20,25,30,10}   ).setMaxHealth(10));
	game->registerCarType(CarType(Car::OLD, "old").setRarities(             {5,20,8,10,15,5,15,20,15,15,5}      ).setMaxHealth(2));
	game->registerCarType(CarType(Car::ARMORED, "lorry").setRarities(       {20,14,30,40,50,15,40,10,12,15,5}   ).setMaxHealth(15));
	game->registerCarType(CarType(Car::TRAIN, "train").setRarities(         {20,50,5,50,25,10,5,30,10,10,20}    ).setMaxHealth(10));
}
