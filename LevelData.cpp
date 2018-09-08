#include "LevelData.h"
#include "maptype.h"
#include "Game.h"

LevelData* Maps::countryside;
LevelData* Maps::desert;
LevelData* Maps::forest;
LevelData* Maps::ice;
LevelData* Maps::mountains;

void LevelData::addRarity(Car::TypeId car, int value)
{
    this->carRarity.at(car) = value;
}

float LevelData::getAcceleration()
{
    return this->acc;
}

int LevelData::getCarRarity(Car::TypeId car)
{
    if(this->carRarity[car] != 0)
        return this->carRarity[car];
    else
        return 10; //default rarity
}

int LevelData::getCarCreationSpeed()
{
	return carCreationSpd == 0 ? 70 : carCreationSpd;
}

sf::Color LevelData::getColor()
{
    return this->mapColor;
}

LevelData::MapType LevelData::getMapType()
{
    return this->mapType;
}

sf::String LevelData::getTextureName()
{
    return this->textureName;
}

LevelData::LevelData(MapType type)
{
    this->mapType = type;
}

void LevelData::init()
{
    Maps::countryside = new LevelData(COUNTRYSIDE);
    Maps::countryside->addRarity(Car::LORRY, 3);
    Maps::countryside->addRarity(Car::RARE, 5);
    Maps::countryside->addRarity(Car::BUS, 10);
    Maps::countryside->addRarity(Car::AMBULANCE, 10);
    Maps::countryside->setAcceleration(4.f);
    Maps::countryside->setColor(sf::Color(82, 133, 75));
    Maps::countryside->setTextureName("countryside");
	Maps::countryside->setCarCreationSpeed(70);

	Maps::desert = new LevelData(LevelData::DESERT);
    Maps::desert->addRarity(Car::LORRY, 3);
    Maps::desert->addRarity(Car::RARE, 2);
    Maps::desert->addRarity(Car::BUS, 3);
    Maps::desert->addRarity(Car::AMBULANCE, 3);
    Maps::desert->setAcceleration(3.4f);
    Maps::desert->setColor(sf::Color::Yellow);
    Maps::desert->setTextureName("desert");
	Maps::desert->setCarCreationSpeed(80);

    Maps::forest = new LevelData(LevelData::FOREST);
    Maps::forest->addRarity(Car::LORRY, 3);
    Maps::forest->addRarity(Car::RARE, 2);
    Maps::forest->addRarity(Car::BUS, 15);
    Maps::forest->addRarity(Car::AMBULANCE, 10);
    Maps::forest->setAcceleration(5.f);
    Maps::forest->setColor(sf::Color(0, 140, 0));
    Maps::forest->setTextureName("forest");
	Maps::forest->setCarCreationSpeed(60);

    Maps::ice = new LevelData(LevelData::ICE);
    Maps::ice->addRarity(Car::LORRY, 15);
    Maps::ice->addRarity(Car::RARE, 3);
    Maps::ice->addRarity(Car::BUS, 8);
    Maps::ice->addRarity(Car::AMBULANCE, 15);
    Maps::ice->setAcceleration(6.f);
    Maps::ice->setColor(sf::Color(230, 230, 230));
    Maps::ice->setTextureName("ice");
	Maps::ice->setCarCreationSpeed(90);

    Maps::mountains = new LevelData(LevelData::MOUNTAINS);
    Maps::mountains->addRarity(Car::LORRY, 13);
    Maps::mountains->addRarity(Car::RARE, 7);
    Maps::mountains->addRarity(Car::BUS, 3);
    Maps::mountains->addRarity(Car::AMBULANCE, 20);
    Maps::mountains->setAcceleration(3.f);
    Maps::mountains->setColor(sf::Color(85, 86, 85));
    Maps::mountains->setTextureName("mountains");
	Maps::mountains->setCarCreationSpeed(85);

	Game::instance->levelRegistry.insert(make_pair("Countryside", Maps::countryside));
	Game::instance->levelRegistry.insert(make_pair("Desert", Maps::desert));
	Game::instance->levelRegistry.insert(make_pair("Forest", Maps::forest));
	Game::instance->levelRegistry.insert(make_pair("Ice", Maps::ice));
	Game::instance->levelRegistry.insert(make_pair("Mountains", Maps::mountains));
}

void LevelData::setAcceleration(float f)
{
    this->acc = f;
}

void LevelData::setCarCreationSpeed(int ccs)
{
	this->carCreationSpd = ccs;
}

void LevelData::setColor(sf::Color c)
{
    this->mapColor = c;
}

void LevelData::setTextureName(sf::String name)
{
    this->textureName = name;
}
