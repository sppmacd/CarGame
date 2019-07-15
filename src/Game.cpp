#include <cmath>
#include <iostream>
#include <fstream>

#include "Game.h"
#include "GameDisplay.h"
#include "CarLorry.h"
#include "CarRare.h"

#include "EventHandler.h"
#include "GameEvent.h"

#include "GuiGameOver.h"

#include "PowerFreeze.hpp"
#include "PowerOil.hpp"
#include "PowerPointBoost.hpp"
#include "PowerFence.hpp"

#include <HackerMan/Util/Main.hpp>

// error codes:
// G00 could not load default language
// [not used] G01 could not load language config
// G02 registering GUIs is deprecated
// G03 second Game instance

Game* Game::instance = NULL;

Game::Game(): GuiHandler(GameDisplay::instance->getRenderWnd(), GameDisplay::instance->getGuiFont())
{
    cout << "Game: Started loading game engine..." << endl;

	GameDisplay::loadingStr = "Loading game data...";

	if (!instance)
	{
		instance = this; //Set main game instance to this

		// Perform first initializations
		this->mainTickCount = 0; //Reset ticking
		this->running = true; //Set game running
		this->pause(true); //Pause game (to not spawn cars!)
		this->debug = false; //Disable debug mode
		this->fullscreen = true;
		//this->isGuiLoaded() = false; //DISABLE GUI!
		this->registerEventHandlers();

		// Reset player stats
		this->isNewPlayer = false;
		this->tutorialStep = 0;

		// Reset level and car data
		LevelData::init();
		CarType::init();

		// Reset powers
		this->powerCooldown = 0;
		this->powerTime = 0;
		this->isPowerUsed = false;
		this->powerHandle = NULL;
		this->unlockedLevels = 0LL;
		this->registerPowers();

		// Load player data
		this->loadPlayerData();

        // Load language configuration
		this->loadLanguages();
	}
	else //fatal error
	{
	    cout << "Game: Tried to create second Game instance!" << endl;
		instance->displayError("G03");
	}
}

void Game::wheelEvent(sf::Event::MouseWheelScrollEvent event)
{
    if(!this->isGuiLoaded() && this->powerTime <= 0)
    {
        if(event.delta > 0.f)
        {
            this->currentPower++;
            if(this->currentPower == powerRegistry.end() || this->currentPower->second->getName().find("generic.") == 0)
                this->currentPower = ++powerRegistry.begin();
        }
        else if(event.delta < 0.f)
        {
            --this->currentPower;
            if(this->currentPower == powerRegistry.begin())
            {
                auto it = powerRegistry.begin();
                for(int s = 0; s < biggestPlayerPowerID; s++) //it isn't random access iterator, so we must increment it
                    it++;
                this->currentPower = it;
            }
        }
    }
}

bool Game::getPower(int id)
{
    return this->powers[id].upgrade(this);
}

bool Game::usePower(int id)
{
    /*if(this->powers[id] > 0)
    {
        this->powers[id]--;
        return true;
    }*/
    return true;
}


void Game::registerPowers()
{
	registerPower(0, (Power*)NULL);
	registerPower(1, &(new PowerOil)->setMaxTime(1800));
	registerPower(2, &(new PowerFreeze)->setMaxTime(600));
	registerPower(3, new PowerPointBoost);
	registerPower(4, new PowerFence);
}

void Game::registerCarType(CarType type)
{
	carTypeRegistry.push_back(type);
}

CarType* Game::findCarTypeByID(Car::TypeId id)
{
    if(!carTypeRegistry.empty())
    {
        for (CarType& type : carTypeRegistry)
        {
            if (type == id)
                return &type;
        }
    }
	return nullptr;
}

float Game::getGameSpeed()
{
	return this->gameSpeed;
}

void Game::setGameSpeed(float speed)
{
	this->gameSpeed = speed;
}

void Game::runEventHandler(Event& event)
{
	int counter = 0;
	for (pair<const Event::EventType, CGEventHandler>& pair : eventHandlers) //must call all event handlers, not only first
	{
		if (pair.first == event.type)
		{
			counter++;
			bool stat = pair.second(event, this);
			if (!stat)
				cout << "Event canceling is not implemented in CG 0.1 MPI!" << endl;
		}
	}

	//if(counter < 1)
		//cout << "Event Handler not found for event " << event.type << endl;
}

bool Game::runGameEventHandler(GameEvent & event)
{
	int counter = 0;
	bool stat = true;
	for (pair<const GameEvent::Type, GameEventHandler>& pair : eventHandlerInst.registry)
	{
		if (pair.first == event.type)
		{
			counter++;
			stat &= pair.second(event, this);
		}
	}

	//if (counter < 1)
		//cout << "Game Event Handler not found for event " << event.type << endl;

	return stat;
}

void Game::registerEventHandlers()
{
	addEventHandler(Event::Closed, EventHandlers::onClose);
	addEventHandler(Event::MouseButtonReleased, EventHandlers::onMouseButtonReleased);
	addEventHandler(Event::KeyPressed, EventHandlers::onKeyPressed);
	addEventHandler(Event::MouseWheelScrolled, EventHandlers::onMouseWheelScrolled);
	//addEventHandler(Event::KeyPressed, EventHandlers::onGUIKeyPressed);
	eventHandlerInst.registerGameEvent(GameEvent::CarSpawning, EventHandlers::onCarSpawning);
}

void Game::addEventHandler(Event::EventType type, CGEventHandler handler)
{
	eventHandlers.insert(make_pair(type, handler));
}

int Game::getCurrentPower()
{
    return this->currentPower == this->powerRegistry.begin() ? 0 : this->currentPower->first;
}

void Game::addScore(int s)
{
    s *= pointMultiplier;
    this->score += s;
    this->totalPlayerPoints += s;
    this->pointsToNewMpl -= s;
}

long Game::getTotalPoints()
{
    return this->totalPlayerPoints;
}

int Game::getCoinMultiplier()
{
    return this->coinMpl;
}

void Game::loadPlayerData()
{
	GameDisplay::loadingStr = "Loading player data...";

    cout << "Game: Loading player data..." << endl;

    HMDataMap playerProfile;
    if(playerProfile.loadFromFile("profile_1.txt"))
    {
        int ver = playerProfile.getNumberKey("version", "", 0);

        cout << "Game: Loading player data from profile version " << ver << endl;
        this->highScore = playerProfile.getNumberKey("highScore", "main", 0);
        this->playerCoins = playerProfile.getNumberKey("coins", "main", 0);

        for(size_t t = 0; t < levelRegistry.size(); t++)
        {
            bool unlocked = playerProfile.getKey("unlocked_" + levelRegistry[t].first, "level", "false") == "true";
            this->unlockedLevels |= (unlocked << t);
        }
        this->totalPlayerPoints = playerProfile.getNumberKey("totalPoints", "main", 0);
        this->coinMpl = playerProfile.getNumberKey("coinMultiplier", "main", 1);
        this->pointsToNewMpl = playerProfile.getNumberKey("pointsToNewMultiplier", "main", 0);

        if(ver == 3)
        {
            for(auto it = powerRegistry.begin(); it != powerRegistry.end(); it++)
            {
                int c = playerProfile.getNumberKey("count_" + to_string(it->first), "power", 0);
                this->powers[it->first] = PowerPlayerData(it->second, sqrt(c));
            }
        }
        else if(ver == 4)
        {
            for(auto it = powerRegistry.begin(); it != powerRegistry.end(); it++)
            {
                int c = playerProfile.getNumberKey("level_" + to_string(it->first), "power", 0);
                this->powers[it->first] = PowerPlayerData(it->second, c);
            }
        }
        else
        {
            cout << "Game: Unsupported profile version!" << endl;
        }
    }
    else
    {
        ifstream file("data.txt");
        if(file.good())
        {
            cout << "Game: Couldn't load profile! Converting from old player data format (v2)..." << endl;

            file
            >> this->highScore
            >> this->playerCoins
            >> this->unlockedLevels
            >> this->totalPlayerPoints
            >> this->coinMpl
            >> this->pointsToNewMpl;
            cout << "Game: Powers are incompatible with old format on " + string(CG_VERSION) + "!" << endl;
        }
        else
        {
            file.open("highscore.txt");

            if(file.good())
            {
                cout << "Game: Couldn't load data.txt! Converting from old player data format (v1)..." << endl;
                file
                >> this->highScore
                >> this->playerCoins
                >> this->unlockedLevels
                >> this->totalPlayerPoints
                >> this->coinMpl
                >> this->pointsToNewMpl;
                cout << "Game: Powers are incompatible with old format on " + string(CG_VERSION) + "!" << endl;
            }
            else
            {
                cout << "Game: Cannot load player data! Creating new profile. You are the new player :)" << endl;
                this->highScore = 0;
                this->playerCoins = 0;
                this->unlockedLevels  = 0;

                this->totalPlayerPoints = 0;
                this->coinMpl = 1;
                this->pointsToNewMpl = 200;

                this->isNewPlayer = true;
                this->tutorialStep = 2;

                for(auto it = powerRegistry.begin(); it != powerRegistry.end(); it++)
                {
                    this->powers[it->first] = PowerPlayerData(it->second);
                }
            }
        }
    }
}

LevelData Game::findLevel(LevelData::MapType type)
{
    return *levelRegistry[type].second;
}

void Game::savePlayerData()
{
    cout << "Game: Saving player data to profile..." << endl;
    GameDisplay::drawLoadingProgress("Saving player data...", GameDisplay::instance->getRenderWnd());

    ofstream file("profile_1.txt");

    file << "// CG " << CG_VERSION << " profile file (version 4)" << endl;
    file << "// Generated by CarGame. Do not edit!" << endl;
    file << "main:highScore=" << this->highScore << endl;
    file << "main:coins=" << this->playerCoins << endl;

    for(size_t t = 0; t < levelRegistry.size(); t++)
    {
        file << "level:unlocked_" << levelRegistry[t].first << "=" << (this->isLevelUnlocked((LevelData::MapType)t) ? "true" : "false") << endl;
    }

    file << "main:totalPoints=" << this->totalPlayerPoints << endl;
    file << "main:coinMultiplier=" << this->coinMpl << endl;
    file << "main:pointsToNewMultiplier=" << this->pointsToNewMpl << endl;

    for(auto it = powerRegistry.begin(); it != powerRegistry.end(); it++)
    {
        file << "power:level_" << it->first << "=" << this->powers[it->first].getLevel() << endl;
    }

    file << ":version=4" << endl;
}

void Game::loadGame(LevelData level)
{
    GameDisplay::drawLoadingProgress("Loading level " + level.getMapType(), GameDisplay::instance->getRenderWnd());

    cout << "Game: Loading level " << level.getTextureName().toAnsiString() << "..." << endl;
    this->level = level;
    setupGame();
}

void Game::loadGame()
{
    GameDisplay::drawLoadingProgress("Reloading current level...", GameDisplay::instance->getRenderWnd());

    cout << "Game: Reloading current level..." << endl;
    setupGame();

}
void Game::setupGame()
{
    this->gameSpeed = this->level.getAcceleration() / (2.2f * 1920.f / GameDisplay::instance->getRenderWnd()->getSize().x);
    this->lastTickScore = 0;
    this->tickCount = 0;
    this->score = 0;
    this->gameOver = false;
    this->cars.clear();
    this->pause(false);
    this->closeGui();
    this->currentPower = ++powerRegistry.begin();
	this->carCreatingSpeed = this->level.getCarCreationSpeed();
	this->newRecord = false;
    this->powerTime = 0;
	this->powerCooldown = 0;
	this->setPointMultiplier((float(this->level.getMapType()) + 2.f) * 0.8f);
}

void Game::closeLevel()
{
    cout << "Game: Closing level..." << endl;
    GameDisplay::drawLoadingProgress("Closing level...", GameDisplay::instance->getRenderWnd());
    if(!this->cars.empty())
    {
        for(Car* car: cars) //fixed memory leak?
            delete car;
        this->cars.clear();
    }
    this->tickCount = 0;
    this->pause(true);
}

Game::~Game()
{
    cout << "Game: Deleting game engine instance..." << endl;
//  this->closeLevel();

	for(auto i: levelRegistry)
        delete i.second;
    for(auto i: powerRegistry)
        delete i.second;
}

void Game::setGameOver()
{
    cout << "Game: Setting game over..." << endl;
    this->gameOver = true;
    this->savePlayerData();
    this->pause(true);
    this->displayGui(new GuiGameOver);

    if(this->powerHandle)
    {
        // Reset powers.
        this->powerHandle->onPowerStop();
        this->powerHandle->onCooldownStop();
    }
}

void Game::pause(bool s)
{
	this->gamePaused = s;
	if(GameDisplay::instance)
		GameDisplay::instance->getRenderWnd()->setKeyRepeatEnabled(s);
}

void Game::toggleFullscreen()
{
    sf::RenderWindow* wnd = GameDisplay::instance->getRenderWnd();

    if(this->fullscreen)
    {
        wnd->create(sf::VideoMode(1280, 720, 32), "CG " + string(CG_VERSION));
        this->fullscreen = false;
    }
    else
    {
		GameDisplay::instance->createFullscreenWnd();
        this->fullscreen = true;
    }
    guiView = View(wnd->getDefaultView()); //bugfix

    if(isGuiLoaded())
    {
        getCurrentGUI()->onResize(); //bugfix v2
    }

	GameDisplay::instance->setVSync(GameDisplay::instance->getVSync()); // fix vsync bug on toggling fullscreen
	GameDisplay::instance->setWndSize(wnd->getSize());
}

void Game::addCoins(long v)
{
    this->playerCoins += v;
}

void Game::removeCoins(long v)
{
    if(this->playerCoins - v >= 0)
        this->playerCoins -= v;
}

long Game::getCoins()
{
    return this->playerCoins;
}

bool Game::isLevelUnlocked(LevelData::MapType type)
{
    return this->unlockedLevels & (0x1 << type);
}

bool Game::isRunning()
{
    return this->running;
}

void Game::exit(int ret)
{
    cout << "Game: Preparing to exit game..." << endl;
    this->running = false;
    this->retVal = ret;
    close(ret);
}

void Game::displayError(string text)
{
    errStr = text;
}

void Game::loadLanguages()
{
    GameDisplay::loadingStr = "Loading language...";
    cout << "Game: Loading language config..." << endl;
    bool b = languageConfig.loadFromFile("config");
    if(!b)
    {
        cout << "Game: Could not load translation config! Creating a new one..." << endl;

        ofstream str("res/lang/config.lang");
        if(str.good())
            str << "current.lang=en_US" << endl;
    } //stream is closed automatically

    bool b2 = enUSTranslation.loadFromFile("en_US");
    if(!b2)
    {
        cout << "Game: Could not load default translation!" << endl;
        displayError("Could not load translation (err 00)");
        return;
    }

    // Load user-defined translation
    string code = languageConfig.get("current.lang");
    bool b1 = translation.loadFromFile(code);
    translation.setParent(&enUSTranslation);
    if(!b1)
    {
        cout << "Game: Could not load user-defined translation file." << endl;
    }
}

float Game::getPointMultiplier()
{
    return pointMultiplier;
}
void Game::setPointMultiplier(float ptmpl)
{
    pointMultiplier = ptmpl;
}

void Game::registerPower(int id, Power* powerInstance)
{
    powerRegistry.insert(make_pair(id, powerInstance));
    if(id < 100 && id > biggestPlayerPowerID)
        biggestPlayerPowerID = id;
}
