#pragma once

#include <vector>
#include "LevelData.h"
#include "Car.h"
#include "GameplayObject.hpp"
#include "ModuleIdentifier.hpp"

using namespace std;

// Class to manage static <Car> data.
class CarType : public GameplayObject
{
	// Car texture name.
	string carTexture;

	// Car max health, passed to <Car>.
	float maxHealth;

	// Car rarities map; it maps "map ID" to car rarity (chance on tick). If 0, car will not spawn.
	map<LevelData::MapType, int> carRarities;

	int defaultRarity;
public:
	// %textureName - The car texture. It will be added to "/res/car/".
	CarType(string textureName);

	// Compare <CarType> with %type2.
	bool operator==(CarType& type2);

	// Compare this TypeId with %id.
	bool operator==(Car::TypeId id);

	// Compare <CarType> with %type2.
	bool operator!=(CarType& type2);

	// Compare this TypeId with %id.
	bool operator!=(Car::TypeId id);

	// Set map rarity on %level to %rarity. If %rarity=0, the car will not spawn.
	CarType* setRarityFor(ModuleIdentifier modId, int rarity);

	// Set default map rarity that is used when no rarity is specified for map.
	CarType* setDefaultRarity(int rarity);

	// Set car max health.
	CarType* setMaxHealth(float max);

	// \returns Texture name of car.
	string getTextureName();

	// \returns The car max health.
	float getMaxHealth();

	// Get rarity for %mapType.
	int getRarity(ModuleIdentifier modId);

	// Current car type ID.
	Car::TypeId carId;
};
