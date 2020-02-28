#include <SFML/Graphics.hpp>
#include "Game.h"
#include "GameDisplay.h"
#include "GameSound.hpp"
#include "DebugLogger.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <exception>

using namespace std;

void loop(Game* game)
{
    GameEvent event;
    event.type = GameEvent::PreTick;
    game->runGameEventHandler(event);

    if(!game->errStr.empty())
    {
        DebugLogger::logDbg("An error occurred in last tick: " + game->errStr);
        game->displayGui(new GuiYesNo("An error occurred: " + game->errStr));
    }

    if(!game->paused())
        game->tickNormalGame();

    game->newTick();
    game->fpsTimer.restart();
    game->initializeGui();

    event.type = GameEvent::PostTick;
    game->runGameEventHandler(event);
}

struct LoadData
{
	sf::RenderWindow* wnd;
	Game* game;
	GameDisplay* disp;
	bool loaded;
};

void loadGame(LoadData* ld)
{
    try
    {
        sf::Clock loadTime;

        cout << "main: Starting Car Game [" << CG_VERSION << "]" << endl;
        cout << "main: Loading game engine..." << endl;

        srand(time(NULL));
        ld->disp = new GameDisplay(ld->wnd);
        GameDisplay::loadingStr = "Loading game engine...";
        ld->game = new Game;

        if(!ld->game->updateFound)
        {
            ld->disp->reload(); // moved from constructor to display loading screen.
            ld->game->sound.reload();
        }

        if(ld->disp->isError())
            throw runtime_error("GameDisplay loading error");

        cout << "main: Loading took " << loadTime.getElapsedTime().asMilliseconds() << "ms." << endl;
        ld->loaded = true;
    }
    catch(exception& e)
    {
        cout << "main: Exception while loading: " << e.what() << endl;
        if(ld->game)
        {
            ld->game->displayError(string("Exception while loading: ") + e.what());
            ld->loaded = true;
        }
        else
        {
            GameDisplay::loadingStr = string("Exception while loading: ") + e.what() + string(".\nPress Esc to close game...");
        }
    }
}

int main()
{
    // redirect SFML error output to null
    //sf::err().rdbuf(NULL);
    LoadData data;
    data.loaded = false;
    data.game = NULL;
    data.disp = NULL;
    int i = 0;

    try
    {
        DebugLogger::logDbg("Creating loading window");
        data.wnd = new RenderWindow(VideoMode(600, 500), "CarGame loading...", Style::None);

        DebugLogger::logDbg("Starting loading thread");
        sf::Thread loadingThread(loadGame,&data);
        loadingThread.launch();

        data.wnd->setActive(true);

        sf::Clock clock;
        sf::Clock eventClock;
        sf::Clock guiClock;
        sf::Clock tickClock;
        sf::Clock renderClock;
        sf::Clock lastWarningClock;
        sf::Event ev1;

        bool mainLoopRunning = true;

        while(mainLoopRunning)
        {
            if(data.loaded)
            {
                try
                {
                    // Initialize loop and check if it should run
                    bool updateDebugStats = data.game->mainTickCount % 6 == 0;
                    if (!data.game->isRunning())
                        mainLoopRunning = false;
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
                    loop(data.game);
                    if (updateDebugStats) data.game->times.timeTick = tickClock.getElapsedTime();

                    // Render game
                    renderClock.restart();
                    data.disp->display();
                    if (updateDebugStats) data.game->times.timeRender = renderClock.getElapsedTime();

                    if (updateDebugStats) data.game->tickTime = clock.getElapsedTime();

                    // Check and notify about lags
                    sf::Uint64 l = clock.getElapsedTime().asMicroseconds();
                    if(l > 16660 && (lastWarningClock.getElapsedTime().asSeconds() > 15.f || l > 40000))
                    {
                        cout << "main: Tick took " << l << endl;
                        lastWarningClock.restart();
                    }

                    // Wait
                    Time waitTime = microseconds(15000) - clock.getElapsedTime();
                    sleep(waitTime);

                    // Post-tick cleanup
                    if (updateDebugStats) data.game->realTickTime = clock.getElapsedTime();
                    if (updateDebugStats) data.game->times.timeWait = waitTime + microseconds(1666);
                }
                catch(bad_alloc& ba)
                {
                    cout << "main: Out of memory!" << endl;
                    if(data.game)
                        data.game->displayError(string("Out of memory!"));
                }
                catch(exception& e)
                {
                    cout << "main: Exception while running: " << e.what() << endl;
                    if(data.game)
                        data.game->displayError(string("Exception while running: ") + e.what());
                }
                catch(...)
                {
                    cout << "main: Unexpected error while running!" << endl;
                    if(data.game)
                        data.game->displayError(string("Unexpected error"));
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
            }
        }

        cout << "main: Closing..." << endl;

        GameDisplay::drawLoadingProgress("Closing...", data.wnd);

        if (data.loaded)
        {
            data.game->savePlayerData();
            i = data.game->retVal;
        }
        else
            i = 0;
    }
    catch(...)
    {
        cout << "main: Unexpected main thread error while loading!" << endl;
    }

    cout << "main: Unloading resources..." << endl;
    delete data.disp;
    delete data.game;

    data.wnd->close();
    delete data.wnd;

    return i;
}
