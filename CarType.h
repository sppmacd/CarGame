#pragma once

#include <vector>
#include "LevelData.h"
using namespace std;

class CarType
{
	string carTexture;
	int maxHealth;
	vector<int> carRarities;
public:
	CarType(Car::TypeId id, string textureName);
	CarType() {}
	bool operator==(CarType& type2);
	bool operator==(Car::TypeId id);
	bool operator!=(CarType& type2);
	bool operator!=(Car::TypeId id);
	CarType& setRarities(initializer_list<int> list);
	CarType& setMaxHealth(int max);
	int getRarity(LevelData::MapType mapType);
	static void init();
	Car::TypeId carId;
};