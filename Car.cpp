#include "Car.h"
#include <iostream>
#include "GameDisplay.h"
#include "Game.h"
#include "CarType.h"

Car::Car(Car::TypeId id, float speed, int line)
    : typeId(id)
    , carSpeed(speed)
    , lineIn(line)
{
    this->pos = 1080.f;
    this->carRelativeToScreen = 1080.f;
    this->destroyTick = -1.f;
    this->canErase = false;
	
	// Create car type for car
	this->type = Game::instance->findCarTypeByID(id);
}

bool Car::tickDestroy()
{
    this->destroyTick -= 0.3;
    if(this->destroyTick < 0.0)
    {
        return true;
    }
    else
    {
        this->colorMultiplier += sf::Color(3,0,0);
        return false;
    }
}

float Car::getSpeed()
{
    return this->carSpeed;
}

int Car::getLine()
{
    return this->lineIn;
}

void Car::setColor(sf::Color color)
{
    this->colorMultiplier = color;
}

void Car::move(float gameSpeed)
{
    this->pos -= this->carSpeed;
    this->carRelativeToScreen -= this->carSpeed / 6.f;
    this->carRelativeToScreen -= gameSpeed / 6.f;
}

sf::Color Car::getColor()
{
    return this->colorMultiplier;
}

void Car::makeDestroy()
{
    if(this->health <= 0)
    {
        this->destroyTick = 20;
        this->carSpeed /= 2.5;
    }
    else
    {
        this->health--;
    }
}

bool Car::isCrashedWith(Car* car)
{
    return car->getLine() == this->getLine() && abs(car->getPos() - this->getPos()) < 50.f;
}

void Car::onCreate()
{
	//...
}

void Car::onDamage()
{
}

void Car::onDestroy()
{
}

void Car::onUpdate()
{
}

sf::Vector2f Car::getScreenPos()
{
    return sf::Vector2f(this->carRelativeToScreen * 2, (this->getLine() * 83) + GameDisplay::instance->getSize().y/2 - 85);
}

string Car::getTextureName()
{
    return this->textureName;
}

void Car::setSpeed(float speed)
{
    this->carSpeed = speed;
}
