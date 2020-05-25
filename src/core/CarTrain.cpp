#include "CarTrain.hpp"

#include "CoreLoader.hpp"

CarTrain::CarTrain(float speed, int lane): Car("train", speed, lane) {}
void CarTrain::onLeave(Game*) {} //nothing - CarTrain is neutral
