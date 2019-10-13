#pragma once

#include <vector>
#include <array>

#include "version.hpp"
#include "UpdateChecker.hpp"

#include "Car.h"
#include "CarType.h"

#include "LevelData.h"
#include "PlayerAbilityManager.hpp"

#include "Power.h"
#include "PowerPlayerData.hpp"

#include "EventHandler.h"

#include "TranslationManager.hpp"
#include "GameSound.hpp"

#include "SettingsManager.hpp"

#include <HackerMan/Util/Main.hpp>
#include <CG/CG.h>
;

#define TUT_NONE 0 //- no tutorial
#define TUT_START 1 //- start game in main menu
#define TUT_BUYMAP 2 //- buy map
#define TUT_SELECTMAP 3 //- select map
#define TUT_DESTROYCAR 4 //- destroy car
#define TUT_DONTLEAVE 5 //- don't let car leave screen
#define TUT_AVOIDBOMB 6 //- avoid bomb
#define TUT_SHOP 7 //- shop

using namespace std;
using namespace sf;
using namespace cg;

class Game: public GuiHandler
{
public:
    struct Times
    {
        sf::Time timeEvent;
        sf::Time timeTick;
        sf::Time timeRender;
        sf::Time timeGui;
        sf::Time timeWait;
    };

	/////////////////////////
	/////// MAIN DATA ///////
	/////////////////////////

	// Instance of the game.
    static Game* instance;

	// Game tick count (since level loaded).
    int tickCount;

	// Main tick count (since the game started).
    long mainTickCount;

	// Variable storing MouseButtonReleased event status.
    bool wasReleased;

    // The settings manager.
	SettingsManager settings;

	// The update checker.
	UpdateChecker updateChecker;
	bool updateFound;

	// GameSound (sound manager) instance.
	GameSound sound;

	// Power time.
    int powerTime;

	// Power cooldown.
    int powerCooldown;

    // Power max time.
    int powerMaxTime;

    // Current power.
    Power* powerHandle;

	// Current level data.
    LevelData level;

	// Current return value for game.
    int retVal;

	// Time values (debug!).
    Times times;

	// Is the power used? (Right click event status).
    bool isPowerUsed;

	// Variable storing car creating speed.
	int carCreatingSpeed;

	// Variable storing, if the new record is set (used in splash screen).
	bool newRecord;

	// Time that elapsed from last car spawn.
    int lastCarTime;

	// Variable storing language settings.
	TranslationManager translation;

	///Vector storing event handlers.
	multimap<sf::Event::EventType, CGEventHandler> eventHandlers;

	/////////////////////////
    ////// PLAYER DATA //////
	/////////////////////////

	// Points required to get new coin multiplier.
    int pointsToNewMpl;

	// Score in the last tick. Used by the game over GUI.
    long lastTickScore;

	// Player highscore, currently used only by renderer.
    long highScore;

	// Variable stores if the player started game first one.
    bool isNewPlayer;

	// Player tutorial step. Can reset by restart.
	// 0 - no tutorial
	// 1 - start game in main menu
	// 2 - buy map
	// 3 - select map
	// 4 - destroy car
	// 5 - avoid bomb
	// 6 - don't let car leave screen
	// 7 - shop
    int tutorialStep;

    // Equipped powers. 0 means no power.
    array<int, 2> equippedPowers;

    // Store player powers (level etc.)
    map<int, PowerPlayerData> powers;

    // Player ability manager - stores abilities.
    PlayerAbilityManager abilities;

    // Loaded from file hmUtil data map.
    HMDataMap otherData;

	/////////////////////////
	////// OTHER DATA ///////
	/////////////////////////

	// Registry of the maps, used by MapSelect GUI.
	vector<pair<string, LevelData*>> levelRegistry;

	// Tick time
    sf::Time tickTime;

	// Real tick time (with wait time)
    sf::Time realTickTime;

	// Debug mode stat
    bool debug;

    // Saved in byte format, unlocked levels for player
    long long unlockedLevels;

	// EventsHandler instance
	EventsHandler eventHandlerInst;

	// Error string
	string errStr;

    // Current player powers
    vector<int> usablePowerIds;

    // Time left to unpause the game (after exiting In-game Menu)
    Time unpauseDelay;

    // Time elapsed from tick start.
    Clock fpsTimer;

    // Time elapsed from last unpause delay splash.
    Clock unpauseSplashTime;

    /////////////////////
	///// FUNCTIONS /////
	/////////////////////

	/// Default constructor
    Game();

	// Default destructor
    ~Game();

	// Vector storing cars
    vector<Car*> cars;

	// Add car to game
    void addCar(Car* car);

	// Called once on LevelTick, updates cars
    void updateCars();

	// Updates power effect
    void updateEffect();

	// Loads specified level
    void loadGame(LevelData level);

	// Reloads current level (after game over)
    void loadGame();

	// Closes current level
    void closeLevel();

	// Ticking normal game (with cars)
    void tickNormalGame();

    // Loads player data from profile or sets to default values if error.
    void loadPlayerData(/*string profileName = "DEFAULT"*/);

    // Saves player data to file.
    void savePlayerData(/*string profileName = "DEFAULT"*/);

	// Start new tick, increment tickCount
    void newTick();

	// Checks if the game is over
    bool isGameOver() { return this->gameOver; }

	// Returns player score
    int getScore() { return this->score; }

	// Sets game to game over stat
    void setGameOver();

	// Pauses / unpauses game
	void pause(bool s);

	// Check if the game is already paused
    bool paused() { return this->gamePaused; }

	// Ticks gui. \param event Last event matching to GUI
    void tickGui(sf::Event& event);

	// Toggles fullscreen
    void toggleFullscreen();

	// Adds specified (v) coins to player
    void addCoins(long v);

	// Removes specified (v) coins to player
    void removeCoins(long v);

	// Returns player coins
    long getCoins();

	// Returns level color, used by renderer
    sf::Color getLevelColor() { return this->level.getColor(); }

	// Checks if the level is unlocked
    bool isLevelUnlocked(LevelData::MapType lvl);

	// Checks if the game is running
    bool isRunning();

	// Sets game state to not running and sets return value to specified value
    void exit(int ret);

	// Returns player total points count. Not used yet
    long getTotalPoints();

	// Returns coin multiplier
    int getCoinMultiplier();

	// Adds score to player
    void addScore(int s);

	// Handles wheel event, used to change power
    void wheelEvent(sf::Event::MouseWheelScrollEvent event);

	// Returns current power ID
    int getCurrentPower();

	// Adds power to player (++)
    bool getPower(int id);

	// Use player power (--)
    bool usePower(int id);

	// Register powers
	void registerPowers();

	// Register car type.
	void registerCarType(CarType type);

	// Find car type by car ID
	CarType* findCarTypeByID(Car::TypeId id);

	// Get game speed value. Used externally by powers
	float getGameSpeed();

	// Set game speed value. Used externally by powers
	void setGameSpeed(float speed);

	// Runs EventHandler for event.
	void runEventHandler(Event& event);

	// Runs GameEventHandler for event.
	bool runGameEventHandler(GameEvent& event);

	// Registers event handlers. Called once on loading.
	void registerEventHandlers();

	// Add event handler.
	void addEventHandler(Event::EventType type, CGEventHandler handler);

	// Find level by ID
	LevelData findLevel(LevelData::MapType type);

	// Display error screen
	void displayError(string text);

	// Loads language list.
	void loadLanguages();

	// Setup game
	void setupGame();

	// Set and initialize current power.
	void setCurrentPower(Power* power);

	// Stop current power and start cooldown.
	void stopCurrentPower();

	// Returns current point multiplier.
	float getPointMultiplier();

	// Sets current point multiplier.
	void setPointMultiplier(float ptmpl);

	// Register a new power, setting biggestPlayerPowerID if necessary.
	void registerPower(int id, Power* powerInstance);

	// Called on load settings registration and load.
	void registerSettings();

	// Open settings GUI.
	void openSettings();

    // Check if
	bool isFullscreen();

	// Called in the first tick, after loading but before any action is performed.
	void postInit();

    // Returns true if specified power is equipped (can use)
	bool isPowerEquipped(int id);

	/////////////////////////////////
	/////////////////////////////////
	/////////////////////////////////

	// Map storing power data registry
	map<int, Power*> powerRegistry;

	// The biggest ID of player power (displayed in Powers GUI)
	int biggestPlayerPowerID;

	// Car type registry
	vector<CarType> carTypeRegistry;

	// All languages.
	TranslationManager languageConfig;

	friend class Triggers;

private:
	// Current game speed
    float gameSpeed;

    // Game speed, with which you started the game.
    float initialGameSpeed;

	// Variable storing game over stat
    bool gameOver;

	// Variable storing game pause stat
    bool gamePaused;

	// Variable storing fullscreen stat
    bool fullscreen;

	// Variable storing game running stat. Almost always true.
    bool running;

	// Total player points, not used yet
    long totalPlayerPoints;

	// Coin multiplier
    int coinMpl;

	// Player coins
    long playerCoins;

	// Player score
    long score;

    // The multiplier applied to points on adding.
    float pointMultiplier;

	// Player current using power (if not used, then 0)
    int currentPower;

    // Variable storing default language (English)
	TranslationManager enUSTranslation;
};
