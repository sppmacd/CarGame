#include "GameLoader.hpp"

#include "GameEvent.h"
#include "Game.h"
#include "GameDisplay.h"
#include "GameSound.hpp"
#include "DebugLogger.hpp"
#include "ArgMap.hpp"

#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <ctime>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

CGAPI GameLoader* GameLoader::instance = NULL;

GameLoader::GameLoader() : game(NULL), disp(NULL), wnd(NULL), loaded(false)
{
    instance = this;
}

// error codes:
// M00 out of memory
// M01 exception
// M02 unexpected exception

void GameLoader::parseArgs(std::map<std::string, std::string>& args, int argc, char* argv[])
{
    std::string key, val; bool keySet = false;
    // add args from cmd line
    for(int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            //add previous arg if keySet
            if(keySet)
            {
                args[key] = val;
            }
            key = argv[i];
            keySet = true;
        }
        else
        {
            val = argv[i];
        }
    }
    if(!key.empty()) args[key] = val; //add last argument
}

void GameLoader::setDefaultArgs(std::map<std::string, std::string>& args)
{
    // set default values
    args["--message"] = "Starting Car Game";
    args["--debug-options"] = "";
}

std::vector<std::string> GameLoader::stringToArgv(std::string str)
{
    std::vector<std::string> vec;
    std::istringstream iss;
    iss.str(str);
    std::string _tmp;
    while(!iss.eof())
    {
        iss >> _tmp;
        vec.push_back(_tmp);
    }
    return vec;
}

void GameLoader::applyArgs(std::map<std::string, std::string>& args)
{
    // convert args to values and save in argmap
    argmap->a_debug_options = args["--debug-options"];
    argmap->a_debug = (args.count("--debug") == 1);
    argmap->a_help = (args.count("--help") == 1);
    argmap->a_info = (args.count("--info") == 1);
    argmap->a_message = args["--message"];
    argmap->a_no_resources =  (args.count("--no-resources") == 1);
    argmap->a_tmp =  (args.count("--tmp") == 1);
}

void GameLoader::loadGame()
{
    loaded = false;
    try
    {
        sf::Clock loadTime;

        DebugLogger::log("" + argmap->a_message + " [" + std::string(CG_VERSION) + "]", "GameLoader");
        DebugLogger::log("Loading game engine...", "GameLoader");

        // display help if specified in cmdline
        if(argmap->a_help)
        {
            GameDisplay::consoleStr =
            "CG " + string(CG_VERSION) + "\n"
            "Command Line Usage:\n"
            "--debug         Starts Car Game in Debug Mode (can be changed in Settings)\n"
            "--debug-options Specifies debug options to be used. See docs/debug_options.md\n"
            "--help          Shows this message\n"
            "--info          Displays game info\n"
            "--message       Sets a custom message displayed in CG log\n"
            "--no-resources  Skips GameDisplay and GameSound resource loading\n"
            "--tmp           Enables Temporary Session Mode (don't save player \n"
            "profile)\n"
            "Press Esc to close game...";
            DebugLogger::log("\n" + GameDisplay::consoleStr, "GameLoader");
            return;
        }
        if(argmap->a_info)
        {
            GameDisplay::consoleStr = "\n"
            "CG " + string(CG_VERSION) + "\n"
            "Debug Information\n"
            "* api-version=" + string("1") + "\n"
            "* arch-prefix='" + string(CG_ARCH_PREFIX) + "'\n"
            "* debug-options='" + string(argmap->a_debug_options) + "'\n"
            "* modules-loaded=" + to_string(modManager->getModuleCount()) + "\n"
            "* profile-version=" + string("5") + "\n"
            "* version-signature='" + string(CG_VERSION_SIG) + "'\n"
            "Press Esc to close game...";
            DebugLogger::log(GameDisplay::consoleStr, "GameLoader");
            return;
        }

        DebugLogger::logDbg("Setting random seed to current timestamp");
        srand(time(NULL));
        DebugLogger::logDbg("Creating GameDisplay");
        disp = new GameDisplay(wnd);
        GameDisplay::loadingStr = "Loading game engine...";
        DebugLogger::logDbg("Creating Game");
        game = new Game(argmap);
        registerEventHandlers();

        if(!game->updateFound)
        {
            DebugLogger::logDbg("Triggering full game reload", "GameLoader");
            disp->reload(argmap->a_no_resources); // moved from constructor to display loading screen.
            game->sound.reload();
        }

        if(disp->isError())
            throw runtime_error("GameDisplay loading error");

        DebugLogger::log("Loading took " + std::to_string(loadTime.getElapsedTime().asMilliseconds()) + " ms.", "GameLoader");
        loaded = true;
    }
    catch(exception& e)
    {
        DebugLogger::log("Exception while loading: " + std::string(e.what()), "GameLoader", "FATAL");
        GameDisplay::consoleStr = string("Exception while loading: ") + e.what() + string(".\nPress Esc to close game...");
    }
}

int GameLoader::main(int argc, char* argv[])
{
    // parse args
    std::map<std::string, std::string> args;

    argmap = new ArgMap;

    setDefaultArgs(args);
    parseArgs(args, argc, argv);
    applyArgs(args);

    // the OS-specific root is expected to create modmanager here.
    preInit();

    int i = 0;
    bool hang = false;

    // Call core handler
    //cgLoad(&data);

    try
    {
        if(!modManager)
        {
            // spawn the default modmanager, it will throw errors later :).
            modManager = new ModuleManager;
        }

        if(!modManager->loadAllModules())
        {
            DebugLogger::log("Couldn't load modules", "GameLoader");
            throw std::runtime_error("M03 - Couldn't load modules");
        }

        DebugLogger::logDbg("Creating loading window", "GameLoader");
        createLoadingWnd();

        DebugLogger::logDbg("Starting loading thread", "GameLoader");
        startLoadingThread();

        sf::Clock clock;
        sf::Clock eventClock;
        //sf::Clock guiClock;
        sf::Clock tickClock;
        sf::Clock renderClock;
        sf::Clock waitClock;
        sf::Clock lastWarningClock;
        //sf::Event ev1;

        bool mainLoopRunning = true;

        while(mainLoopRunning)
        {
            // to handle closing game by GuiHandler::close()
            if(game && !game->isRunning())
                mainLoopRunning = false;

            if(loaded)
            {
                try
                {
                    // Initialize loop and check if it should run
                    bool updateDebugStats = /*data.game->mainTickCount % 6 == 0*/ true;
                    //bool mouseMoveHandled = false;

                    // Restart clocks
                    clock.restart();
                    eventClock.restart();
                    if(updateDebugStats) game->times.timeGui = Time::Zero;

                    // Call postInit()
                    if(game->mainTickCount == 0)
                    {
                        DebugLogger::logDbg("Starting first-tick initialization", "GameLoader");
                        game->postInit();
                    }

                    // Check all events
                    checkEvents();
                    if (updateDebugStats) game->times.timeEvent = eventClock.getElapsedTime();

                    // Update game logic
                    tickClock.restart();
                    loop(game);
                    if (updateDebugStats) game->times.timeTick = tickClock.getElapsedTime();

                    // Render game
                    renderClock.restart();
                    disp->display();
                    if (updateDebugStats) game->times.timeRender = renderClock.getElapsedTime();

                    if (updateDebugStats) game->tickTime = clock.getElapsedTime();

                    // Check and notify about lags
                    waitClock.restart();
                    sf::Uint64 l = clock.getElapsedTime().asMicroseconds();
                    if(l > 16660 && (lastWarningClock.getElapsedTime().asSeconds() > 15.f || l > 40000))
                    {
                        DebugLogger::log("Tick took " + std::to_string(l) + "us.", "GameLoader", "LAG");
                        lastWarningClock.restart();
                    }

                    // Wait
                    Time waitTime = microseconds(15000) - clock.getElapsedTime();
                    sleep(waitTime);

                    // Post-tick cleanup
                    if (updateDebugStats) game->times.timeWait = waitClock.getElapsedTime();
                    if (updateDebugStats) game->realTickTime = clock.getElapsedTime();

                    hang = false;
                }
                catch(bad_alloc& ba)
                {
                    if(game && !hang)
                    {
                        DebugLogger::log("Out of memory!", "GameLoader", "FATAL");
                        game->displayError("Out of memory!", "M00");
                        hang = true;
                    }
                    else
                    {
                        DebugLogger::log("Out of memory!", "GameLoader", "FATAL");
                        DebugLogger::log("occurred after another exception! Stopping game.", "GameLoader", "FATAL");
                        break;
                    }
                }
                catch(exception& e)
                {
                    if(game && !hang)
                    {
                        DebugLogger::log("std::exception caught: " + std::string(e.what()), "GameLoader", "FATAL");
                        game->displayError("Exception while running: " + std::string(e.what()), "M01");
                        hang = true;
                    }
                    else
                    {
                        DebugLogger::log("std::exception caught: " + std::string(e.what()), "GameLoader", "FATAL");
                        DebugLogger::log("occurred after another exception! Stopping game.", "GameLoader", "FATAL");
                        break;
                    }
                }
                catch(...)
                {
                    if(game && !hang)
                    {
                        DebugLogger::log("Unexpected exception caught!", "GameLoader", "FATAL");
                        game->displayError("Unexpected error", "M02");
                    }
                    else
                    {
                        DebugLogger::log("Unexpected exception caught!", "GameLoader", "FATAL");
                        DebugLogger::log("occurred after another exception", "GameLoader", "FATAL");
                        break;
                    }
                }
            }
            else
            {
                bool b = loadingCheckEvents();
                if(!b)
                    return 0;
                GameDisplay::drawLoading(wnd);
                if(!GameDisplay::consoleStr.isEmpty())
                    sf::sleep(sf::seconds(1));
            }
        }

        DebugLogger::log("Closing the game...", "GameLoader");
        GameDisplay::drawLoadingProgress("Closing...", wnd);

        if(loaded)
        {
            game->savePlayerData();
            i = game->retVal;
        }
        else
            i = 0;
    }
    catch(...)
    {
        DebugLogger::log("Unexpected exception caught from main(). This shouldn't happen.", "GameLoader", "FATAL");
    }

    DebugLogger::log("Unloading resources...", "GameLoader");
    cleanup();

    return i;
}

void GameLoader::loop(Game* game)
{
    GameEvent event;
    event.type = GameEvent::PreTick;
    game->runGameEventHandler(event);

    if(!game->paused() && !game->error)
        game->tickNormalGame();

    game->newTick();
    game->fpsTimer.restart();
    game->initializeGui();

    event.type = GameEvent::PostTick;
    game->runGameEventHandler(event);
}

void GameLoader::consoleColor(std::string level)
{
    static bool warned = false;
    if(!warned)
    {
        warned = true;
        std::cout << "DebugLogger: Console coloring not supported" << std::endl;
    }
    std::hex(std::cout);
    std::cout << "&" << level << " ";
    std::dec(std::cout);
}

void GameLoader::cleanup()
{
    delete disp;
    delete game;

    wnd->close();
    delete wnd;
    delete argmap;
}

#define EMPTY {}

void GameLoader::preInit()                  EMPTY
void GameLoader::createLoadingWnd()         EMPTY
void GameLoader::startLoadingThread()       EMPTY
void GameLoader::registerEventHandlers()    EMPTY
void GameLoader::checkEvents()              EMPTY
bool GameLoader::loadingCheckEvents()       EMPTY
