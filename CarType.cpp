#include "CarType.h"
#include "Game.h"

CarType::CarType(int id, string textureName) : carId(id), carTexture(textureName)
{
}

bool CarType::operator==(CarType & type2)
{
	return type2.carId == this->carId;
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

void CarType::init()
{
	Game* game = Game::instance;
	game->registerCarType(Car::NORMAL, CarType(Car::NORMAL, "default").setRarities({2,2,2,2,2}));
	game->registerCarType(Car::LORRY, CarType(Car::LORRY, "lorry").setRarities({3,3,3,15,13}));
	game->registerCarType(Car::RARE, CarType(Car::RARE, "default").setRarities({5,2,2,3,7}));
	game->registerCarType(Car::BUS, CarType(Car::BUS, "bus").setRarities({10,3,14,8,3}));
	game->registerCarType(Car::AMBULANCE, CarType(Car::AMBULANCE, "ambulance").setRarities({10,3,10,15,20}));

	// TODO: NEW CARS!
	game->registerCarType(Car::RALLY, CarType(Car::RALLY, "default").setRarities({2,20,5,25,10}));
	game->registerCarType(Car::FIREMAN, CarType(Car::FIREMAN, "default").setRarities({5,7,3,10,8}));
	game->registerCarType(Car::TANK, CarType(Car::TANK, "default").setRarities({10,7,15,20,25}));
	game->registerCarType(Car::OLD, CarType(Car::OLD, "default").setRarities({5,20,8,10,15}));
	game->registerCarType(Car::BOMB, CarType(Car::BOMB, "default").setRarities({30,30,30,30,30}));
}
