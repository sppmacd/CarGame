#ifndef GAMELOADER_HPP
#define GAMELOADER_HPP

#include "Game.h"
#include "GameDisplay.h"
#include "ModuleManager.hpp"

#include <cargame/Export.hpp>

#include <SFML/Graphics.hpp>

class GameLoader
{
public:
    // from 0.2 LoadData
	Game* game;
	GameDisplay* disp;
	bool loaded;
	ArgMap* argmap;
	sf::RenderWindow* wnd;
	ModuleManager* modManager;

	// The instance.
	CGAPI static GameLoader* instance;

	// Default constructor.
	GameLoader();

	// Destructor
	virtual ~GameLoader() {}

    // Program entry.
	virtual int main(int argc, char* argv[]);

    // Thread entry for game loading.
	virtual void loadGame();

	// Game loop.
	virtual void loop(Game* game);

	// Converts main() argument format to map format (key value) or (boolean)
    // By convention, all keys are starting with '--' or '-' (for aliases).
    void parseArgs(std::map<std::string, std::string>& args, int argc, char* argv[]);

    // Simple convert string to argv (argument vector)
    // It doesn't support e.g. quotes.
    std::vector<std::string> stringToArgv(std::string str);

    // Saves default values of arguments to map.
    virtual void setDefaultArgs(std::map<std::string, std::string>& args);

    // Applies parsed args to ArgMap.
    virtual void applyArgs(std::map<std::string, std::string>& args);

    /////////////////////////////////////////////////////////
	// FUNCTIONS TO IMPLEMENT BY OS-SPECIFIC ROOT LAUNCHER //
	/////////////////////////////////////////////////////////

    // Does pre-initialization of game, basing on arguments
    // on desktop OS's: redirects sfml error output to NULL if not debug mode
    // The OS-specific ROOT is expected to create its ModuleManager here; if not,
    // the mods won't be loaded.
    virtual void preInit();

    // Creates loading window (splash or fullscreen, depending on OS)
    virtual void createLoadingWnd();

    // Starts loading thread.
    virtual void startLoadingThread();

    // Function called by Game to register OS-specific event handlers.
    virtual void registerEventHandlers();

    // Checks events and passes it to Game.
    virtual void checkEvents();

    // Check events of loading phase.
    // On desktop OS's, [Esc] or window close aborts loading.
    // Returns false if game should not be running.
    virtual bool loadingCheckEvents();

    // Does cleanup. Game and GameDisplay is now invalid, so you can't use it
    // If you want to do module cleanup (Game and GameDisplay are not deleted yet),
    // implement cgCleanup function.
    virtual void cleanup();

    // Sets console color. OS-specific.
    virtual void consoleColor(std::string level);
};

// GameLoader for desktop OS's.
class DesktopGameLoader : public GameLoader
{
public:
    DesktopGameLoader();
};

#endif // GAMELOADER_HPP
