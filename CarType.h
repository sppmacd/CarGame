#pragma once

#include <vector>
#include "LevelData.h"
using namespace std;

class CarType
{
	string carTexture;
	int carId;
	vector<int> carRarities;
public:
	CarType(int id, string textureName);
	CarType() {}
	bool operator==(CarType& type2);
	bool operator!=(CarType& type2);
	CarType& setRarities(initializer_list<int> list);
};