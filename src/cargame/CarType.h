#pragma once

#include <vector>
#include "LevelData.h"
#include "Car.h"
using namespace std;

// Class to manage static <Car> data.
class CarType
{
	// Car texture name.
	string carTexture;

	// Car max health, passed to <Car>.
	float maxHealth;

	// Car rarities map; it maps "map ID" to car rarity (chance on tick). If 0, car will not spawn.
	map<LevelData::MapType, int> carRarities;
public:
	// %textureName - The car texture. It will be added to "/res/car/".
	CarType(string textureName);

	CarType() {}

	// Compare <CarType> with %type2.
	bool operator==(CarType& type2);

	// Compare this TypeId with %id.
	bool operator==(Car::TypeId id);

	// Compare <CarType> with %type2.
	bool operator!=(CarType& type2);

	// Compare this TypeId with %id.
	bool operator!=(Car::TypeId id);

	// Set standard rarities to %list.
	CarType* setRarities(initializer_list<int> list);

	// Set map rarity on %level to %rarity. If %rarity=0, the car will not spawn.
	CarType* setRarityFor(LevelData::MapType level, int rarity);

	// Set car max health.
	CarType* setMaxHealth(float max);

	// \returns Texture name of car.
	string getTextureName();

	// \returns The car max health.
	float getMaxHealth();

	// Get rarity for %mapType.
	int getRarity(LevelData::MapType mapType);

	// Current car type ID.
	Car::TypeId carId;
};
