#include "CarTrain.hpp"

CarTrain::CarTrain(float speed, int lane): Car(Car::TRAIN, speed, lane) {}
void CarTrain::onLeave(Game*) {} //nothing - CarTrain is neutral
