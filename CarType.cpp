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
	carRarities.resize(LevelData::COUNT);
	std::copy(list.begin(), list.end(), carRarities.begin());

	if (carRarities.size() < LevelData::COUNT)
		for (int i = list.size(); i < LevelData::COUNT; i++)
			carRarities.push_back(1);

	return *this;
}

CarType & CarType::setMaxHealth(int max)
{
	this->maxHealth = max;
	return *this;
}

string CarType::getTextureName()
{
	return carTexture;
}

int CarType::getMaxHealth()
{
	return maxHealth;
}

int CarType::getRarity(LevelData::MapType mapType)
{
	return carRarities[mapType];
}

void CarType::init()
{
	Game* game = Game::instance;
	game->registerCarType(CarType(Car::NORMAL, "default").setRarities({2,2,2,2,2,2}).setMaxHealth(1));
	game->registerCarType(CarType(Car::LORRY, "lorry").setRarities({3,3,3,15,13,5}).setMaxHealth(3));
	game->registerCarType(CarType(Car::RARE, "default").setRarities({5,3,3,4,7,8}).setMaxHealth(2));
	game->registerCarType(CarType(Car::BUS, "bus").setRarities({10,3,14,8,3,3}).setMaxHealth(5));
	game->registerCarType(CarType(Car::AMBULANCE, "ambulance").setRarities({10,3,10,15,20,5}).setMaxHealth(4));

	// TODO: NEW CARS!
	game->registerCarType(CarType(Car::RALLY, "rally").setRarities({4,20,5,25,10,20}).setMaxHealth(2));
	game->registerCarType(CarType(Car::BOMB, "bomb").setRarities({15,15,15,15,15,15}).setMaxHealth(1));

	game->registerCarType(CarType(Car::FIREMAN, "fireman").setRarities({5,7,3,10,8,12}).setMaxHealth(5));
	game->registerCarType(CarType(Car::TANK, "tank").setRarities({10,7,15,20,25,10}).setMaxHealth(10));
	game->registerCarType(CarType(Car::OLD, "default").setRarities({5,20,8,10,15,5}).setMaxHealth(2));
	game->registerCarType(CarType(Car::ARMORED, "lorry").setRarities({20,14,30,40,50,15}).setMaxHealth(20));
}
