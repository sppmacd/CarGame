#pragma once

#include <vector>
#include "LevelData.h"
#include "Car.h"
using namespace std;

class CarType
{
	string carTexture;
	float maxHealth;
	vector<int> carRarities;
public:
	CarType(Car::TypeId id, string textureName);
	CarType() {}
	bool operator==(CarType& type2);
	bool operator==(Car::TypeId id);
	bool operator!=(CarType& type2);
	bool operator!=(Car::TypeId id);
	CarType& setRarities(initializer_list<int> list);
	CarType& setMaxHealth(float max);
	string getTextureName();
	float getMaxHealth();
	int getRarity(LevelData::MapType mapType);
	static void init();
	Car::TypeId carId;
};
