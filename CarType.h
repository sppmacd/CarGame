#pragma once

#include <vector>
#include "LevelData.h"
using namespace std;

class CarType
{
	string carTexture;
	int carId;
	int maxHealth;
	vector<int> carRarities;
public:
	CarType(int id, string textureName);
	CarType() {}
	bool operator==(CarType& type2);
	bool operator!=(CarType& type2);
	CarType& setRarities(initializer_list<int> list);
	CarType& setMaxHealth(int max);
	static void init();
};