////////////////////////////
// Car Game ROOT Launcher //
// (cg.exe)               //
////////////////////////////

#include <cargame/DebugLogger.hpp>
#include <cargame/ArgMap.hpp>
#include <cargame/GameLoader.hpp>

#include <string>
#include <map>

#include <SFML/Graphics.hpp>

// core handler
//#include <core/CoreLoader.hpp>

using namespace std;

// todo: document loading progress and mark load state for all virtual functions

// error codes:
// M00 out of memory
// M01 exception
// M02 unexpected exception

int main(int argc, char* argv[])
{
    // parse args
    std::map<std::string, std::string> args;
    std::string key, val; bool keySet = false;
    ArgMap argmap;

    // set default values
    args["--message"] = "Starting Car Game";

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

    // convert args to values and save in argmap
    argmap.a_debug = (args.count("--debug") == 1);
    argmap.a_help = (args.count("--help") == 1);
    argmap.a_message = args["--message"];

    // redirect SFML error output to null if not debug mode
    if(!argmap.a_debug) sf::err().rdbuf(NULL);

    GameLoader data;
    data.loaded = false;
    data.game = NULL;
    data.disp = NULL;
    data.argmap = &argmap;
    int i = 0;

    // Call core handler
    //cgLoad(&data);

    try
    {
        DebugLogger::logDbg("Creating loading window");
        data.wnd = new RenderWindow(VideoMode(600, 500), "CarGame loading...", Style::None);

        DebugLogger::logDbg("Starting loading thread");
        sf::Thread loadingThread(GameLoader::loadGame, &data);
        loadingThread.launch();

        data.wnd->setActive(true);

        sf::Clock clock;
        sf::Clock eventClock;
        sf::Clock guiClock;
        sf::Clock tickClock;
        sf::Clock renderClock;
        sf::Clock waitClock;
        sf::Clock lastWarningClock;
        sf::Event ev1;

        bool mainLoopRunning = true;

        while(mainLoopRunning)
        {
            // to handle closing game by GuiHandler::close()
            if(data.game && !data.game->isRunning())
                mainLoopRunning = false;

            if(data.loaded)
            {
                try
                {
                    // Initialize loop and check if it should run
                    bool updateDebugStats = /*data.game->mainTickCount % 6 == 0*/ true;
                    bool mouseMoveHandled = false;

                    // Restart clocks
                    clock.restart();
                    eventClock.restart();
                    if(updateDebugStats) data.game->times.timeGui = Time::Zero;

                    // Call postInit()
                    if(data.game->mainTickCount == 0)
                    {
                        DebugLogger::logDbg("Starting first-tick initialization");
                        data.game->postInit();
                    }


                    // Check all events
                    while(data.wnd->pollEvent(ev1))
                    {
                        // Call event handler
                        if(ev1.type != Event::MouseMoved || !mouseMoveHandled)
                        {
                            //DebugLogger::logDbg("Event:" + std::to_string(ev1.type));
                            data.game->runEventHandler(ev1);
                            mouseMoveHandled = true;
                        }

                        // Tick GUI for each event
                        guiClock.restart();
                        data.game->handleEvent(ev1); // run CGUI handler
                        if (updateDebugStats) data.game->times.timeGui += guiClock.getElapsedTime();
                    }
                    if (updateDebugStats) data.game->times.timeEvent = eventClock.getElapsedTime();

                    // Update game logic
                    tickClock.restart();
                    data.loop(data.game);
                    if (updateDebugStats) data.game->times.timeTick = tickClock.getElapsedTime();

                    // Render game
                    renderClock.restart();
                    data.disp->display();
                    if (updateDebugStats) data.game->times.timeRender = renderClock.getElapsedTime();

                    if (updateDebugStats) data.game->tickTime = clock.getElapsedTime();

                    // Check and notify about lags
                    waitClock.restart();
                    sf::Uint64 l = clock.getElapsedTime().asMicroseconds();
                    if(l > 16660 && (lastWarningClock.getElapsedTime().asSeconds() > 15.f || l > 40000))
                    {
                        DebugLogger::log("Tick took " + std::to_string(l) + "us.", "main", "LAG");
                        lastWarningClock.restart();
                    }

                    // Wait
                    Time waitTime = microseconds(15000) - clock.getElapsedTime();
                    sleep(waitTime);

                    // Post-tick cleanup
                    if (updateDebugStats) data.game->times.timeWait = waitClock.getElapsedTime();
                    if (updateDebugStats) data.game->realTickTime = clock.getElapsedTime();
                }
                catch(bad_alloc& ba)
                {
                    DebugLogger::log("Out of memory!", "main", "FATAL");
                    if(data.game)
                        data.game->displayError("Out of memory!", "M00");
                }
                catch(exception& e)
                {
                    DebugLogger::log("std::exception caught: " + std::string(e.what()), "main", "FATAL");
                    if(data.game)
                        data.game->displayError("Exception while running: " + std::string(e.what()), "M01");
                }
                catch(...)
                {
                    DebugLogger::log("Unexpected exception caught!", "main", "FATAL");
                    if(data.game)
                        data.game->displayError("Unexpected error", "M02");
                }
            }
            else
            {
                while (data.wnd->pollEvent(ev1))
                {
                    if (ev1.type == Event::Closed || (ev1.type == Event::KeyPressed && ev1.key.code == Keyboard::Escape))
                        return 0;
                }
                GameDisplay::drawLoading(data.wnd);
                if(!GameDisplay::consoleStr.isEmpty())
                    sf::sleep(sf::seconds(1));
            }
        }

        DebugLogger::log("Closing the game...", "main");
        GameDisplay::drawLoadingProgress("Closing...", data.wnd);

        if(data.loaded)
        {
            data.game->savePlayerData();
            i = data.game->retVal;
        }
        else
            i = 0;
    }
    catch(...)
    {
        DebugLogger::log("Unexpected exception caught from main(). This shouldn't happen.", "main", "FATAL");
    }

    DebugLogger::log("Unloading resources...", "main");
    delete data.disp;
    delete data.game;

    data.wnd->close();
    delete data.wnd;

    return i;
}