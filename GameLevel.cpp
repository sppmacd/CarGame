#include "Game.h"
#include "CarLorry.h"
#include "CarRare.h"
#include "CarBus.h"
#include "CarAmbulance.h"
#include "GameSound.h"
#include "GameDisplay.h"

void Game::addCar(Car* car)
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
            Car* car = this->cars[i];
            car->move(this->gameSpeed / 0.176);
			car->onUpdate(this);

            if(car->isDestroying())
            {
                if(car->tickDestroy())
                {
                    car->setToErase();
                }
            }
            else
            {
                if(car->pos < -50.f)
                {
                    car->onLeave(this);
                    car->setToErase();
                    continue;
                }

                if(abs(car->getScreenPos().x - GameDisplay::instance->mousePos().x) < 100.f && abs(car->getScreenPos().y - GameDisplay::instance->mousePos().y) < 40.f && this->wasReleased)
                {
					car->onDamage(this);

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
        this->wasReleased = false;
    }
}

void Game::tickNormalGame()
{
    if(this->tickCount % this->carCreatingSpeed == 0)
    {
        Car car;

		vector<CarType*> selectedTypes;
		while(selectedTypes.empty())
		for (CarType& type : carTypeRegistry)
		{
			if (rand() % type.getRarity(this->level.getMapType()) == 0)
				selectedTypes.push_back(&type);
		}

		CarType* carType = selectedTypes[rand() % selectedTypes.size()];

		// Create event
		GameEvent event;
		event.type = GameEvent::CarSpawning;
		event.carSpawned.carToCreate = &car;
		event.carSpawned.type = carType;
		bool createCar = runGameEventHandler(event);

		if (createCar)
		{
			addCar(event.carSpawned.carToCreate);
			car.onCreate(this);
			//delete &car; //deallocate memory allocated in EventHandler
		}
    }

    this->updateCars();

    if(this->score > this->highScore)
    {
		if (!this->newRecord)
		{
			this->newRecord = true; // New record!
			//Create splash screen

			if(this->highScore != 0)
				GameDisplay::instance->setSplash(translation.get("splash,newrecord"));
		}

		//Update highscore to score
        this->highScore = this->score;
    }

    this->updateEffect();

    this->moveCamera();

    if(this->pointsToNewMpl <= 0)
    {
        this->coinMpl++;
        this->pointsToNewMpl = this->getCoinMultiplier() * 200;
    }

    if(this->gameOver && !this->paused())
    {
        this->pause(true);
    }
}

void Game::newTick()
{
    if(!this->isGuiLoaded)
    {
        this->gameSpeed += this->level.getAcceleration() / 5000;
        ++tickCount;
    }

    if(this->guiCooldown > 0)
        this->guiCooldown--;

    this->mainTickCount++;
}

void Game::updateEffect()
{
    if(this->isPowerUsed && this->powerCooldown <= 0 && this->getCurrentPower() != 0)
    {
        auto it = this->powerRegistry.find(this->getCurrentPower());
        if(it == this->powerRegistry.end())
            return;

        powerHandle = it->second;
        this->powerTime = powerHandle->maxPowerTime;
        this->powerMaxTime = this->powerTime;
		this->powerCooldown = -1;
        this->usePower(this->currentPower);

		// power 'start'
		if (!powerHandle->onPowerStart())
		{
			this->powerTime = 0;
			this->powerCooldown = 0;
			this->powers[this->getCurrentPower()]++; //Reset power count to previous
		}
    }
    if(this->powerTime > 0)
    {
        this->powerTime--;

		// power 'tick'
		powerHandle->onPowerTick(this->powerTime);

        if(this->powerTime == 1)
        {
            this->powerCooldown = 300; // 3 seconds
            this->powerTime = -1; //0 - can use power, -1 - cooldown, >0 - power is used, 1 - set cooldown!

			// power 'stop'
			powerHandle->onPowerStop();
        }
    }

    else if(this->powerTime == -1)
    {
		// power 'cooldown tick'
		powerHandle->onCooldownTick(this->powerCooldown);
        this->powerCooldown--;
    }

    if(this->powerCooldown == 1) // 1-set CUP, >1-cooldown!, -1-power is using!, 0-can use power
    {
		// power 'cooldown stop'
		powerHandle->onCooldownStop();
        this->powerTime = 0;
		this->powerCooldown = 0;
		powerHandle = NULL;
    }
    this->isPowerUsed = false;
}
