#include <cmath>
#include <iostream>
#include <fstream>

#include "Game.h"
#include "GameDisplay.h"
#include "CarLorry.h"
#include "CarRare.h"

#include "EventHandler.h"
#include "GameEvent.h"

#include "DebugLogger.hpp"

#include "GuiGameOver.h"
#include "GuiLanguage.hpp"
#include "GuiSettings.h"
#include "GuiIngame.h"
#include "GuiMainMenu.h"
#include "GuiUpdates.hpp"

#include "PowerFreeze.hpp"
#include "PowerOil.hpp"
#include "PowerPointBoost.hpp"
#include "PowerFence.hpp"
#include "PowerBall.hpp"
#include "PowerSpeedIncrease.hpp"
#include "PowerDamageDecrease.hpp"

#include <HackerMan/Util/Main.hpp>

// error codes:
// G00 could not load default language
// [not used] G01 could not load language config
// G02 registering GUIs is deprecated
// G03 second Game instance
// G04 invalid profile

Game* Game::instance = NULL;

Game::Game(ArgMap* argmap): GuiHandler(GameDisplay::instance->getRenderWnd(), GameDisplay::instance->getGuiFont())
{
    cout << "Game: Started loading game engine..." << endl;

	GameDisplay::loadingStr = "Loading game data...";

	if (!instance)
	{
	    DebugLogger::logDbg("Performing basic setup", "Game");
		instance = this; //Set main game instance to this
		this->running = true; //Set game running

		// Perform first initializations
		DebugLogger::logDbg("Checking for updates", "Game");
		GameDisplay::loadingStr = "Checking for updates...";
		this->updateFound = this->updateChecker.checkUpdates();

		DebugLogger::logDbg("Loading game API", "Game");
		GameDisplay::loadingStr = "Loading game engine...";
		this->registerSettings();
		this->mainTickCount = 0; //Reset ticking
		this->pause(true); //Pause game (to not spawn cars!)
		this->debug = stoi(settings.getSetting("debug", "global")) || argmap->a_debug; //Disable debug mode
		this->fullscreen = true;//stoi(settings.getSetting("fullscreen", "graphics"));
		this->registerEventHandlers();
		cg::colors::bgColor = sf::Color(50, 40, 40);
		cg::colors::textSize = 28;

		// Reset player stats
		DebugLogger::logDbg("Initializing player profile", "Game");
		this->isNewPlayer = false;
		this->tutorialStep = 0;

		// Initialize registries
		DebugLogger::logDbg("Starting registry filling", "Game");
		LevelData::init();
		CarType::init();
		this->abilities.init();

		// Reset powers
		DebugLogger::logDbg("Starting power setup", "Game");
		this->powerCooldown = 0;
		this->powerTime = 0;
		this->isPowerUsed = false;
		this->powerHandle = NULL;
		this->currentPower = 0;
		this->unlockedLevels = 0LL;
		this->registerPowers();

		// Load player data
		DebugLogger::logDbg("Loading player data", "Game");
		this->loadPlayerData();

        // Load language configuration
        DebugLogger::logDbg("Loading language config", "Game");
		this->loadLanguages();

		DebugLogger::logDbg("Last initializations", "Game");
		this->tickCount = 0;
	}
	else //fatal error
	{
	    cout << "Game: Tried to create second Game instance!" << endl;
		instance->displayError("G03");
	}
}

void Game::wheelEvent(sf::Event::MouseWheelScrollEvent event)
{
    if(!this->isGuiLoaded() && this->powerTime <= 0 && !usablePowerIds.empty())
    {
        if(event.delta > 0.f)
        {
            this->currentPower++;
            if(this->currentPower == usablePowerIds.size())
                this->currentPower = 0;
        }
        else if(event.delta < 0.f)
        {
            --this->currentPower;
            if(this->currentPower == -1)
                this->currentPower = usablePowerIds.size() - 1;
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
	//registerPower(0, (Power*)NULL);
	biggestPlayerPowerID = 0;
	biggestGenericPowerID = 100;
	registerPower(1, &(new PowerOil)->setMaxTime(1800));
	registerPower(2, &(new PowerFreeze)->setMaxTime(3000));
	registerPower(3, new PowerPointBoost);
	registerPower(4, new PowerFence);
	registerPower(5, new PowerBall);
	registerPower(101, new PowerSpeedIncrease);
	registerPower(102, new PowerDamageDecrease);
}

void Game::registerCarType(CarType type)
{
	carTypeRegistry.push_back(type);
}

CarType* Game::findCarTypeByID(Car::TypeId id)
{
    if(!carTypeRegistry.empty())
    {
        for(CarType& type : carTypeRegistry)
        {
            if(type == id)
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
	for(pair<const Event::EventType, CGEventHandler>& pair : eventHandlers) //must call all event handlers, not only first
	{
		if(pair.first == event.type)
		{
			counter++;
			bool stat = pair.second(event, this);
			if(!stat)
				cout << "Game: Cannot cancel system events!!!" << endl;
		}
	}

	//if(counter < 1)
		//cout << "Event Handler not found for event " << event.type << endl;
}

bool Game::runGameEventHandler(GameEvent& event)
{
    //DebugLogger::logDbg("Called event: " + to_string(int(event.type)), "Game");
	int counter = 0;
	bool stat = true;
	for(pair<const GameEvent::Type, GameEventHandler>& pair : eventHandlerInst.registry)
	{
		if(pair.first == event.type)
		{
			counter++;
			stat &= pair.second(event, this);
		}
	}
	/*if(!stat)
        DebugLogger::logDbg("Canceled event: " + to_string(int(event.type)), "Game");*/

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

	eventHandlerInst.registerGameEvent(GameEvent::CarSpawning, EventHandlers::onCarSpawning);
	eventHandlerInst.registerGameEvent(GameEvent::CarDamaged, EventHandlers::onCarDamaged);
}

void Game::addEventHandler(Event::EventType type, CGEventHandler handler)
{
	eventHandlers.insert(make_pair(type, handler));
}

int Game::getCurrentPower()
{
    return usablePowerIds.size() > 0 ? usablePowerIds[this->currentPower] : -1;
}

void Game::addScore(int s)
{
    if(s > 0)
    {
        s *= pointMultiplier;
        this->score += s;
        this->totalPlayerPoints += s;
        this->pointsToNewMpl -= s;
        sound.playSound("point_add", 75.f);
        GameDisplay::instance->resetPointAnim();
    }
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

    this->usablePowerIds.clear();

    otherData = HMDataMap(); //clear all data
    if(otherData.loadFromFile("profile_1.txt"))
    {
        int ver = otherData.getNumberKey("version", "", 0);

        cout << "Game: Loading player data from profile version " << ver << endl;
        this->highScore = otherData.getNumberKey("highScore", "main", 0);
        this->playerCoins = otherData.getNumberKey("coins", "main", 0);

        for(size_t t = 0; t < levelRegistry.size(); t++)
        {
            bool unlocked = otherData.getKey("unlocked_" + levelRegistry[t].first, "level", "false") == "true";
            this->unlockedLevels |= (unlocked << t);
        }
        this->totalPlayerPoints = otherData.getNumberKey("totalPoints", "main", 0);
        this->coinMpl = otherData.getNumberKey("coinMultiplier", "main", 1);
        this->pointsToNewMpl = otherData.getNumberKey("pointsToNewMultiplier", "main", 0);

        if(ver == 3)
        {
            for(auto it = powerRegistry.begin(); it != powerRegistry.end(); it++)
            {
                int c = otherData.getNumberKey("count_" + to_string(it->first), "power", 0);
                this->powers[it->first] = PowerPlayerData(it->second, sqrt(c));
            }
        }
        else if(ver == 4)
        {
            for(auto it = powerRegistry.begin(); it != powerRegistry.end(); it++)
            {
                int c = otherData.getNumberKey("level_" + to_string(it->first), "power", 0);
                this->powers[it->first] = PowerPlayerData(it->second, c);
            }
            for(size_t t = 0; t < equippedPowers.size(); t++)
            {
                equippedPowers[t] = otherData.getNumberKey("power_" + to_string(t), "equipment", 0);
                if(equippedPowers[t] != 0)
                    usablePowerIds.push_back(equippedPowers[t]);
            }
        }
        else
        {
            cout << "Game: Unsupported profile version!" << endl;
            displayError("Tried to load newer/invalid profile version.\n\
                         Try installing a new version of Car Game.\n\
                         Code: G04");
            initProfile();
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
            int i1, i2;
            file
            >> i1 >> i2;
            this->powers[1] = PowerPlayerData(this->powerRegistry[1], sqrt(i1));
            this->powers[2] = PowerPlayerData(this->powerRegistry[2], sqrt(i2));
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
                initProfile();
            }
        }
    }
    //this->abilities = PlayerAbilityManager();
    this->abilities.read(otherData);
}

void Game::initProfile()
{
    cout << "Game: Cannot load player data! Creating a new profile. You are the new player :)" << endl;
    this->highScore = 0;
    this->playerCoins = 0;
    this->unlockedLevels  = 0;

    this->totalPlayerPoints = 0;
    this->coinMpl = 1;
    this->pointsToNewMpl = 200;

    this->isNewPlayer = true;
    this->tutorialStep = 1;

    for(auto it = powerRegistry.begin(); it != powerRegistry.end(); it++)
    {
        this->powers[it->first] = PowerPlayerData(it->second);
    }
    for(size_t t = 0; t < equippedPowers.size(); t++)
    {
        equippedPowers[t] = 0;
    }
    abilities.clear();
}

LevelData Game::findLevel(LevelData::MapType type)
{
    return *levelRegistry[type].second;
}

void Game::savePlayerData()
{
    cout << "Game: Saving player data to profile..." << endl;
    GameDisplay::drawLoadingProgress("Saving player data...", GameDisplay::instance->getRenderWnd());

    // Save using hmUtil
    otherData.setNumberKey("version", 4, "");

    otherData.setNumberKey("highScore", highScore, "main");
    otherData.setNumberKey("coins", playerCoins, "main");
    otherData.setNumberKey("totalPoints", totalPlayerPoints, "main");
    otherData.setNumberKey("coinMultiplier", coinMpl, "main");
    otherData.setNumberKey("pointsToNewMultiplier", pointsToNewMpl, "main");

    for(size_t t = 0; t < equippedPowers.size(); t++)
    {
        otherData.setNumberKey("power_" + to_string(t), equippedPowers[t], "equipment");
    }
    for(size_t t = 0; t < levelRegistry.size(); t++)
    {
        otherData.setKey("unlocked_" + levelRegistry[t].first, (isLevelUnlocked((LevelData::MapType)t) ? "true" : "false"), "level");
    }
    for(auto it = powerRegistry.begin(); it != powerRegistry.end(); it++)
    {
        otherData.setNumberKey("level_" + to_string(it->first), powers[it->first].getLevel(), "power");
    }
    abilities.write(otherData);

    otherData.saveToFile("profile_1.txt");
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
    GameEvent event;
    event.type = GameEvent::LevelLoadingStart;
    event.level.level = &this->level;
    runGameEventHandler(event);

    this->sound.playSound("start", 100.f);
    this->gameSpeed = this->level.getAcceleration() / (2.2f * 1920.f / GameDisplay::instance->getRenderWnd()->getSize().x);
    this->initialGameSpeed = this->gameSpeed;
    this->lastTickScore = 0;
    this->lastCarTime = 1;
    this->tickCount = 1;
    this->score = 0;
    this->gameOver = false;
    this->cars.clear();
    //this->pause(false);
    //this->closeGui();
    this->currentPower = 0;
	this->carCreatingSpeed = this->level.getCarCreationSpeed();
	this->newRecord = false;
    this->powerTime = 0;
	this->powerCooldown = 0;
	this->setPointMultiplier((float(this->level.getMapType()) + 2.f) * 0.8f);
	this->setDamageMultiplier(this->abilities.calculateValue(PlayerAbilityManager::DAMAGE));
	this->unpauseGame(3.f);
}

void Game::closeLevel()
{
    cout << "Game: Closing level..." << endl;
    GameDisplay::drawLoadingProgress("Closing level...", GameDisplay::instance->getRenderWnd());
    if(!this->cars.empty())
    {
        for(Car* car: cars)
            delete car;
        this->cars.clear();
    }
    this->tickCount = 0;

    if(this->powerHandle)
    {
        // Reset powers.
        this->powerHandle->onPowerStop();
        this->powerHandle->onCooldownStop();

        // fix bug with powers not started if closed level during power time.
        powerHandle = NULL;
    }
    this->pause(true);
}

Game::~Game()
{
    cout << "Game: Deleting game engine instance..." << endl;

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
    this->displayGui(new GuiGameOver);
    this->sound.playSound("game_over", 100.f);
    this->closeLevel();

    if(isNewPlayer)
    {
        if(tutorialStep >= TUT_AVOIDBOMB) //bomb or click car
        {
            tutorialStep = TUT_SHOP;
        }
        else
        {
            tutorialStep = TUT_DONTLEAVE;
        }
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
    sound.playSound("coin_add", 75.f);
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
    DebugLogger::logDbg("An error occurred in last tick: " + errStr);
    displayGui(new GuiYesNo("An error occurred: " + errStr));
    mainTickCount = 2; //to skip post-init
}

void Game::loadLanguages()
{
    GameDisplay::loadingStr = "Loading language...";
    cout << "Game: Loading language config..." << endl;
    bool b = hmLangCfg.loadFromFile("lang.hmd");
    if(!b)
    {
        cout << "Game: Could not load translation config! Creating a new one..." << endl;

        hmLangCfg.setKey("current","en_US","lang");
        hmLangCfg.saveToFile("lang.hmd");

        // try to load newly created config
        b = hmLangCfg.loadFromFile("lang.hmd");
        if(!b)
        {
            displayError("Error: G01: Could not load language config");
        }
    }

    bool b2 = enUSTranslation.loadFromFile("en_US");
    if(!b2)
    {
        cout << "Game: Could not load default translation!" << endl;
        displayError("Error: G00: Could not load default translation");
        return;
    }

    // Load user-defined translation
    string code = hmLangCfg.getKey("current","lang","en_US");
    bool b1 = translation.loadFromFile(code);
    translation.setParent(&enUSTranslation);
    if(!b1)
    {
        cout << "Game: Could not load user-defined translation file." << endl;
        currentLangCode = "en_US";
    }
    else
    {
        currentLangCode = code;
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
    DebugLogger::logDbg("Adding power: " + std::to_string(id) + " (" + powerInstance->getName() + ")", "Game");
    powerInstance->id = id;
    powerRegistry.insert(make_pair(id, powerInstance));
    if(id <= 100)
    {
        if(id > biggestPlayerPowerID)
            biggestPlayerPowerID = id;
    }
    else
    {
        if(id > biggestGenericPowerID)
            biggestGenericPowerID = id;
    }
}

class Triggers
{
public:
    static bool s_refreshres(string)
    {
        GameDisplay::instance->reload();
        Game::instance->sound.reload();
        return true;
    }
    static bool s_verticalsync(string val)
    {
        GameDisplay::instance->setVSync(stoi(val));
        return true;
    }
    static bool s_fullscreen(string val)
    {
        if(Game::instance->fullscreen != stoi(val))
            Game::instance->toggleFullscreen();
        //Game::instance->fullscreen = stoi(val);
        return true;
    }
    static bool s_resetgame(string)
    {
		remove("data.txt");
		remove("highscore.txt");
		remove("profile_1.txt");
		Game::instance->loadPlayerData();
		Game::instance->displayGui(new GuiMainMenu);
		return true;
    }
    static bool s_language(string)
    {
        Game::instance->displayedGui->runDialog(new GuiLanguage, -1);
        return true;
    }
    static bool s_volume(string val)
    {
        Game::instance->sound.soundVolume = stof(val);
        return true;
    }
    static bool s_debug(string val)
    {
        Game::instance->debug = stoi(val);
        return true;
    }
	static bool s_resetsettings(string)
	{
		Game::instance->settings.resetSettings("settings.txt");
		GuiSettings* gs = dynamic_cast<GuiSettings*>(Game::instance->getCurrentGUI());
		if(gs) gs->reset();
		Game::instance->settings.addWidgetsToSettings(gs);
		return true;
	}
};

void Game::registerSettings()
{
    settings.registerSetting("refreshres", SettingsManager::TRIGGER, "graphics");
    settings.registerSetting("verticalsync", SettingsManager::BOOLEAN, "graphics", "1");
    settings.registerSetting("fullscreen", SettingsManager::BOOLEAN, "graphics", "1");
    settings.registerSetting("resetgame", SettingsManager::CONFIRM_TRIGGER, "game");
    settings.registerSetting("language", SettingsManager::TRIGGER, "global");
    settings.registerSetting("resetsettings", SettingsManager::CONFIRM_TRIGGER, "global");
    settings.registerSetting("debug", SettingsManager::BOOLEAN, "global", "0");
    settings.registerSetting("volume", SettingsManager::NUMERIC, "sound", "0.5");

    settings.registerTrigger("refreshres", Triggers::s_refreshres, "graphics");
    settings.registerTrigger("verticalsync", Triggers::s_verticalsync, "graphics");
    settings.registerTrigger("fullscreen", Triggers::s_fullscreen, "graphics");
    settings.registerTrigger("resetgame", Triggers::s_resetgame, "game");
    settings.registerTrigger("language", Triggers::s_language, "global");
    settings.registerTrigger("resetsettings", Triggers::s_resetsettings, "global");
    settings.registerTrigger("debug", Triggers::s_debug, "global");
    settings.registerTrigger("volume", Triggers::s_volume, "sound");

    settings.loadSettings("settings.txt");
}

void Game::openSettings()
{
    GuiSettings* settingsGui = settings.generateWidgets();

    if(instanceof_ptr(displayedGui, GuiMainMenu))
        settingsGui->returnGui = new GuiMainMenu;
    else if(instanceof_ptr(displayedGui, GuiIngame))
        settingsGui->returnGui = new GuiIngame;

    displayGui(settingsGui);
}

bool Game::isFullscreen()
{
    return fullscreen;
}

void Game::postInit()
{
    DebugLogger::logDbg("Performing post-init", "Game");
    /*else
    {
        GameDisplay::instance->getRenderWnd()->create(sf::VideoMode(1280, 720, 32), "CG " + string(CG_VERSION));
    }*/

    // Display the main menu
    if(updateFound)
    {
        displayGui(new GuiUpdates(&updateChecker));
    }
    else
    {
        displayGui(new GuiMainMenu);
    }
    settings.triggerAllClose();
    if(isFullscreen())
        GameDisplay::instance->createFullscreenWnd();

    //GameDisplay::instance->createFullscreenWnd();
}

bool Game::isPowerEquipped(int id)
{
    for(size_t s = 0; s < equippedPowers.size(); s++)
    {
        if(equippedPowers[s] == id)
        {
            return true;
        }
    }
    return false;
}

void Game::unpauseGame(float time)
{
    closeGui();
    unpauseDelay = sf::seconds(time);
    GameDisplay::instance->setSplash(to_string(int(unpauseDelay.asSeconds())) + "...");
    unpauseSplashTime.restart(); //reset splash to display counter properly
}

bool Game::canPowerBuyOrEquip()
{
    bool powerCanBuy = false;
    bool powerCanEquip = false;
    int unlockedPowers = 0;
    for(auto it: Game::instance->powerRegistry)
    {
        if(it.first != 0 && it.first < 100)
        {
            if(Game::instance->powers[it.first].getLevel() > 0)
                unlockedPowers++;
            else if(Game::instance->powers[it.first].getUpgradeCost() <= Game::instance->getCoins())
                powerCanBuy = true;
        }
    }
    if(Game::instance->usablePowerIds.size() < 2 && unlockedPowers > 1)
        powerCanEquip = true;
    return powerCanBuy || powerCanEquip;
}

void Game::setDamageMultiplier(float dmg)
{
    if(dmg < 0.f)
        dmg = 0.f; //don't allow negative
    damageMultiplier = dmg;
}

float Game::getDamageMultiplier()
{
    return damageMultiplier;
}
