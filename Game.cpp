#include "Game.h"
#include "CarLorry.h"
#include "CarRare.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include "GameDisplay.h"

#include "Gui.h"
#include "GuiGameOver.h"
#include "GuiIngame.h"
#include "GuiMainMenu.h"
#include "GuiSettings.h"
#include "GuiMapSelect.h"
#include "GuiPowers.h"
#include "GuiYesNo.h"

#include "EventHandler.h"
#include "GameEvent.h"

// error codes:
// 00 could not load default language
// 01 could not load language config

Game* Game::instance = NULL;

Game::Game()
    : displayedGui(nullptr)
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
		this->isGuiLoaded = false; //DISABLE GUI!
		this->registerEventHandlers();

		// Reset player stats
		this->isNewPlayer = false;
		this->tutorialStep = 0;
		this->powers = new int[3];

		// Reset level and car data
		LevelData::init();
		CarType::init();

		// Reset powers
		this->powerCooldown = 0;
		this->powerTime = 0;
		this->isPowerUsed = false;
		this->registerPowers();

		// Load player data
		this->loadPlayerData();

        // Load language configuration
		this->loadLanguages();
	}
	else //fatal error
	{
	    cout << "Game: Tried to create second Game instance!" << endl;
		instance->exit(-1);
	}
}

void Game::wheelEvent(sf::Event::MouseWheelScrollEvent event)
{
    int powercount = 2;

    if(!this->isGuiLoaded && this->powerTime <= 0)
    {
        if(event.delta > 0.f)
        {
            this->currentPower++;
            if(this->currentPower > powercount) this->currentPower = 1;
        }
        else if(event.delta < 0.f)
        {
            --this->currentPower;
            if(this->currentPower < 1) this->currentPower = powercount;
        }
    }
}

void Game::getPower(int id)
{
    this->powers[id]++;
}

void Game::usePower(int id)
{
    if(this->powers[id] > 0)
        this->powers[id]--;
}

void Game::registerGUIs()
{
    cout << "Game: Registering GUIs is deprecated" << endl;
}

void Game::registerPowers()
{
	powerRegistry.insert(make_pair(0, PowerHandles()));
	powerRegistry.insert(make_pair(1, PowerHandles(PowerOil::drawPower, PowerOil::onPowerStart, PowerOil::onPowerStop, PowerOil::onPowerTick, PowerOil::drawPowerIdle).setMaxTime(1800)));
	powerRegistry.insert(make_pair(2, PowerHandles(PowerFreeze::drawPower, PowerFreeze::onPowerStart, PowerFreeze::onPowerStop).setMaxTime(600)));
}

void Game::registerCarType(CarType type)
{
	carTypeRegistry.push_back(type);
}

CarType* Game::findCarTypeByID(Car::TypeId id)
{
	for (CarType& type : carTypeRegistry)
	{
		if (type == id)
			return &type;
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
	for (pair<const Event::EventType, EventHandler>& pair : eventHandlers)
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
	for (pair<const GameEvent::Type, GameEventHandler>& pair : eventHandler.registry)
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
	addEventHandler(Event::KeyPressed, EventHandlers::onGUIKeyPressed);
	eventHandler.registerGameEvent(GameEvent::CarSpawning, EventHandlers::onCarSpawning);
}

void Game::addEventHandler(Event::EventType type, EventHandler handler)
{
	eventHandlers.insert(make_pair(type, handler));
}

int  Game::getCurrentPower()
{
    return this->currentPower;
}

void Game::addScore(int s)
{
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

    std::ifstream file;
    file.open("data.txt");

    if(file.good())
    {
        cout << "Game: Player data read from data.txt" << endl;

        file
        >> this->highScore
        >> this->playerCoins
        >> this->unlockedLevels
        >> this->totalPlayerPoints
        >> this->coinMpl
        >> this->pointsToNewMpl
        >> this->powers[1]
        >> this->powers[2];
    }
    else
    {
        file.open("highscore.txt");

        if(file.good())
        {
            cout << "Game: Couldn't load data.txt! Converting from old player data format..." << endl;
            file
            >> this->highScore
            >> this->playerCoins
            >> this->unlockedLevels
            >> this->totalPlayerPoints
            >> this->coinMpl
            >> this->pointsToNewMpl
            >> this->powers[1]
            >> this->powers[2];
        }
        else
        {
            cout << "Game: Cannot load player data! Creating new profile. You are the new player :)" << endl;
            this->highScore = 0;
            this->playerCoins = 0;
            this->unlockedLevels = 0b0;
            this->totalPlayerPoints = 0;
            this->coinMpl = 1;
            this->pointsToNewMpl = 200;
            this->isNewPlayer = true;
            this->tutorialStep = 2;
            this->powers = new int[3];
            this->powers[1] = 0;
            this->powers[2] = 0;
        }
    }
}

LevelData Game::findLevel(LevelData::MapType type)
{
    return *levelRegistry[type].second;
}

void Game::savePlayerData()
{
    cout << "Game: Saving player data in data.txt..." << endl;

    GameDisplay::drawLoadingProgress("Saving player data...", GameDisplay::instance->getRenderWnd());

    std::ofstream file;
    file.open("data.txt");

    file << this->highScore << " "
    << this->getCoins() << " "
    << this->unlockedLevels << " "
    << this->totalPlayerPoints << " "
    << this->coinMpl << " "
    << this->pointsToNewMpl << " "
    << this->powers[1] << " "
    << this->powers[2] << " ";
}

void Game::loadGame(LevelData level)
{
    GameDisplay::drawLoadingProgress("Loading level " + level.getMapType(), GameDisplay::instance->getRenderWnd());

    cout << "Game: Loading level " << level.getMapType() << "..." << endl;

    this->level = level;

    this->lastTickScore = 0;
    this->cameraPos = 0;
    this->tickCount = 0;
    this->gameSpeed = level.getAcceleration() / 2.2f;
    this->score = 0;
    this->gameOver = false;
    this->cars.clear();
    this->pause(false);
    this->closeGui();
    this->currentPower = 0;
	this->carCreatingSpeed = level.getCarCreationSpeed();
	this->newRecord = false; //Set new record to false

	this->powerTime = 0;
	this->powerCooldown = 0;
}

void Game::loadGame()
{
    GameDisplay::drawLoadingProgress("Reloading current level...", GameDisplay::instance->getRenderWnd());

    cout << "Game: Reloading current level..." << endl;

    this->lastTickScore = 0;
    this->cameraPos = 0;
    this->tickCount = 0;
    this->gameSpeed = this->level.getAcceleration() / 2.2f;
    this->score = 0;
    this->gameOver = false;
    this->cars.clear();
    this->pause(false);
    this->closeGui();
    this->currentPower = 0;
	this->carCreatingSpeed = this->level.getCarCreationSpeed();
	this->newRecord = false; //Set new record to false

	this->powerTime = 0;
	this->powerCooldown = 0;
}

void Game::closeLevel()
{
    cout << "Game: Closing level..." << endl;
    GameDisplay::drawLoadingProgress("Closing level...", GameDisplay::instance->getRenderWnd());
    if(!this->cars.empty()) this->cars.clear();
    this->tickCount = 0;
    this->pause(true);
}

Game::~Game()
{
    cout << "Game: Deleting game engine instance..." << endl;
//  this->closeLevel();
	delete[] this->powers;

	for(auto i: levelRegistry)
        delete i.second;
}

void Game::tickGui(sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved)
	{
		sf::Vector2i mousepos(event.mouseMove.x, event.mouseMove.y);
		this->tickEventMouseMove(GameDisplay::instance->getRenderWnd()->mapPixelToCoords(mousepos));
	}

    if(event.type == sf::Event::MouseButtonReleased)
    {
		sf::Vector2i mousepos(event.mouseButton.x, event.mouseButton.y);
        this->tickEventMouseClick(GameDisplay::instance->getRenderWnd()->mapPixelToCoords(mousepos));
    }
}

void Game::tickEventMouseMove(sf::Vector2f pos)
{
    if(this->isGuiLoaded)
    {
		this->displayedGui->onMouseMove(pos);
    }
}

void Game::tickEventMouseClick(sf::Vector2f pos)
{
    if(this->isGuiLoaded)
    {
		Button b = this->displayedGui->onMouseClick(pos);
		if(!(Button() == b))
        {
            this->displayedGui->onButton(b.id);
        }
    }
}

void Game::setGameOver()
{
    cout << "Game: Setting game over..." << endl;
    this->gameOver = true;
    this->savePlayerData();
    this->pause(true);
    this->displayGui(new GuiGameOver);
}

void Game::pause(bool s)
{
	this->gamePaused = s;
	if(GameDisplay::instance)
		GameDisplay::instance->getRenderWnd()->setKeyRepeatEnabled(s);
}

void Game::moveCamera()
{
    this->cameraPos += static_cast<int>(this->gameSpeed * 4.5f);
}

void Game::displayGui(Gui* gui)
{
	this->closeGui(); //Close previous GUI

	if(gui != nullptr)
	{
	    this->isGuiLoaded = true;
	    this->displayedGui = gui;
	    this->displayedGui->onLoad();
	}
}

void Game::closeGui()
{
	if (this->isGuiLoaded)
	{
	    this->displayedGui->onClose();
	    delete this->displayedGui;
		this->displayedGui = nullptr;
		this->isGuiLoaded = false;
	}
}

void Game::toggleFullscreen()
{
    sf::RenderWindow* wnd = GameDisplay::instance->getRenderWnd();

    if(this->fullscreen)
    {
        wnd->create(sf::VideoMode(1280, 720, 32), "Car Game");
        this->fullscreen = false;
    }
    else
    {
		wnd->create(sf::VideoMode::getFullscreenModes()[0], sf::String(), sf::Style::Fullscreen);
        this->fullscreen = true;
    }
	GameDisplay::instance->setVSync(GameDisplay::instance->getVSync()); // to fix vsync bug on toggling fullscreen
	GameDisplay::instance->setWndSize(wnd->getSize());
}

void Game::addCoins(long v)
{
    this->playerCoins += v;
}

void Game::removeCoins(long v)
{
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
    cout << "Game: Handling exit()..." << endl;
    this->running = false;
    this->retVal = ret;
}

void Game::displayError(string text)
{
    errStr = text;
}

void Game::loadLanguages()
{
    cout << "Game: Loading language config..." << endl;
    bool b = languageConfig.loadFromFile("config");
    if(!b)
    {
        cout << "Game: Could not load translation config!" << endl;
        displayError("Could not load translation config (err 01)");
    }

    // Load user-defined translation
    string code = languageConfig.get("current.lang");
    bool b1 = translation.loadFromFile(code);
    if(!b1)
    {
        // Load default translation (English)
        cout << "Game: Could not load translation file. Defaulting to en_US" << endl;
        bool b2 = translation.loadFromFile("en_US");
        if(!b2)
        {
            cout << "Game: Could not load default translation!" << endl;
            displayError("Could not load translation (err 00)");
        }
    }
}
