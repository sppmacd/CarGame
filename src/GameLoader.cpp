#include "GameLoader.hpp"

#include "GameEvent.h"
#include "Game.h"
#include "GameDisplay.h"
#include "GameSound.hpp"
#include "DebugLogger.hpp"
#include "ArgMap.hpp"

#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <exception>

void GameLoader::loadGame()
{
    try
    {
        sf::Clock loadTime;

        cout << "main: " + argmap->a_message + " [" << CG_VERSION << "]" << endl;
        cout << "main: Loading game engine..." << endl;

        // display help if specified in cmdline
        if(argmap->a_help)
        {
            GameDisplay::consoleStr =
            "CG " + string(CG_VERSION) + "\n"
            "Command Line Usage:\n"
            "--debug     Starts Car Game in Debug Mode (can be changed in Settings)\n"
            "--message   Sets a custom message displayed in CG log (really only to test\n"
            "command line args)\n"
            "--help      Shows this message\n"
            "Press Esc to close game...";
            DebugLogger::log(GameDisplay::consoleStr, "main");
            return;
        }

        DebugLogger::logDbg("Setting random seed to current timestamp");
        srand(time(NULL));
        DebugLogger::logDbg("Creating GameDisplay");
        disp = new GameDisplay(wnd);
        GameDisplay::loadingStr = "Loading game engine...";
        DebugLogger::logDbg("Creating Game");
        game = new Game(argmap);

        if(!game->updateFound)
        {
            DebugLogger::logDbg("Triggering full game reload");
            disp->reload(); // moved from constructor to display loading screen.
            game->sound.reload();
        }

        if(disp->isError())
            throw runtime_error("GameDisplay loading error");

        cout << "main: Loading took " << loadTime.getElapsedTime().asMilliseconds() << "ms." << endl;
        loaded = true;
    }
    catch(exception& e)
    {
        cout << "main: Exception while loading: " << e.what() << endl;
        GameDisplay::loadingStr = string("Exception while loading: ") + e.what() + string(".\nPress Esc to close game...");
    }
}

void GameLoader::loop(Game* game)
{
    GameEvent event;
    event.type = GameEvent::PreTick;
    game->runGameEventHandler(event);

    if(!game->paused())
        game->tickNormalGame();

    game->newTick();
    game->fpsTimer.restart();
    game->initializeGui();

    event.type = GameEvent::PostTick;
    game->runGameEventHandler(event);
}
