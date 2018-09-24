#include "CarType.h"

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
}
