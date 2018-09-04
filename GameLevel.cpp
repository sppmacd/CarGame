#include "Game.h"
#include "CarLorry.h"
#include "CarRare.h"
#include "CarBus.h"
#include "CarAmbulance.h"
#include "GameSound.h"

void Game::addCar(Car car)
{
    this->cars.push_back(car);
}

void Game::updateCars()
{
    this->lastTickScore = this->score;

    if(!this->cars.empty())
    {
        for(unsigned int i = 0; i < this->cars.size(); i++)
        {
            Car* car = &(this->cars[i]);
            car->move(this->gameSpeed / 0.176);

            if(car->isDestroying())
            {
                if(car->tickDestroy())
                {
                    car->setToErase();
                }
            }
            else
            {
                if(car->carRelativeToScreen < -50.f)
                {
                    car->setToErase();
                    this->setGameOver();
                    this->closeLevel();
                    continue;
                }

                if(abs(car->getScreenPos().x - sf::Mouse::getPosition().x) < 100.f && abs(car->getScreenPos().y - sf::Mouse::getPosition().y) < 40.f && this->wasReleased)
                {
                    car->makeDestroy();
                    if(car->typeId == Car::RARE)
                    {
                        this->addScore(2);
                    }
                    else
                    {
                        this->addScore(1);
                    }

                    if(this->score % 2 == 0)
                    {
                        this->addCoins(this->getCoinMultiplier());
                    }

                    if(this->tutorialStep == 5)
                    {
                        this->tutorialStep = 6;
                    }

                    continue;
                }
            }

            if(car->canErase)
            {
                this->cars.erase(this->cars.begin() + i);
            }
        }
        this->wasReleased = false; // This was moved from destroy code to prevent bug (gos2) with destroying
    }
}

void Game::tickNormalGame()
{
	if (this->carCreatingSpeed == 0) this->carCreatingSpeed = this->level.getCarCreationSpeed();
    if(this->tickCount % this->carCreatingSpeed == 0)
    {
        Car car;

        if(rand() % this->level.getCarRarity(Car::LORRY) == 0)
        {
            car = CarLorry(1.7f, rand() % 3);
            int c = rand() % 56 + 200;
            car.setColor(sf::Color(c, c, c));
        }
        else if(rand() % this->level.getCarRarity(Car::RARE) == 0)
        {
            car = CarRare(1.7f, rand() % 3);
            int c = rand() % 56 + 200;
            car.setColor(sf::Color(c, 50, 50));
        }
        else if(rand() % this->level.getCarRarity(Car::BUS) == 0)
        {
            car = CarBus(1.7f, rand() % 3);
            int c = rand() % 200 + 56;
            car.setColor(sf::Color(c, c, 50));
        }
        else if(rand() % this->level.getCarRarity(Car::AMBULANCE) == 0)
        {
            car = CarAmbulance(1.7f, rand() % 3);
            car.setColor(sf::Color(230, 230, 230));
        }
        else
        {
            car = Car(1.7f, rand() % 3);
            car.setColor(sf::Color(((rand() % 8)*32)-1, ((rand() % 8)*32)-1, ((rand() % 8)*32)-1));
        }

        this->addCar(car);
    }

    this->updateCars();

    if(this->score > this->highScore)
    {
        this->highScore = this->score;
    }

    this->updateEffect();

    this->moveCamera();

    if(this->pointsToNewMpl <= 0)
    {
        this->coinMpl++;
        this->pointsToNewMpl = this->getCoinMultiplier() * 200;
    }
}

void Game::newTick()
{
    if(!this->isGuiLoaded)
    {
        this->gameSpeed += this->level.getAcceleration() / 5000;

		if (this->tickCount % 100 == 9 && this->carCreatingSpeed > 30)
			this->carCreatingSpeed--;

        ++tickCount;
    }

    if(this->guiCooldown > 0)
        this->guiCooldown--;

    this->mainTickCount++;
}

void Game::updateEffect()
{
    if(this->isPowerUsed && this->powerCooldown <= 0)
    {
        this->powerTime = 200;
		this->powerCooldown = -1;
        this->usePower(this->currentPower);

		// power 'start'
		if (!this->powerRegistry.find(this->getCurrentPower())->second.onPowerStart())
		{
			this->powerTime = 0;
			this->powerCooldown = 0;
		}
    }
    if(this->powerTime > 0)
    {
        this->powerTime--;

		// power 'tick'
		this->powerRegistry.find(this->getCurrentPower())->second.onPowerTick(this->powerTime);

        if(this->powerTime == 1)
        {
            this->powerCooldown = 300; // 3 seconds
            this->powerTime = -1; //0 - can use power, -1 - cooldown, >0 - power is used, 1 - set cooldown!

			// power 'stop'
			this->powerRegistry.find(this->getCurrentPower())->second.onPowerStop();
        }
    }

    else if(this->powerTime == -1)
    {
		// power 'cooldown tick'
		this->powerRegistry.find(this->getCurrentPower())->second.onCooldownTick(this->powerCooldown);
        this->powerCooldown--;
    }

    if(this->powerCooldown == 1) // 1-set CUP, >1-cooldown!, -1-power is using!, 0-can use power
    {
		// power 'cooldown stop'
		this->powerRegistry.find(this->getCurrentPower())->second.onCooldownStop();
        this->powerTime = 0;
		this->powerCooldown = 0;
    }
    this->isPowerUsed = false;
}
