#include "GameLoader.h"

#include "GuiMainMenu.h"
#include "GuiUpdates.hpp"
//#include "GameSound.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <exception>
#include <fstream>

using namespace std;

namespace Loader
{
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
                reloadAllResources();
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
    void loop(Game* game)
    {
        if (game->mainTickCount == 0)
        {
            if(game->isFullscreen())
                GameDisplay::instance->createFullscreenWnd();
            else
                GameDisplay::instance->getRenderWnd()->create(VideoMode(700, 700), string("CG ") + CG_VERSION);

            // Display the main menu
            if(game->updateFound)
            {
                game->displayGui(new GuiUpdates(&game->updateChecker));
            }
            else
            {
                game->displayGui(new GuiMainMenu);
            }
        }

        if(!game->errStr.empty())
                game->displayGui(new GuiYesNo("An error occured: " + game->errStr));

        if(!game->paused())
            game->tickNormalGame();

        game->newTick();
        game->initializeGui();
    }
    int run()
    {
        //redirect sfml errors and log to separate files.
        std::ofstream sfErrs("err.log");
        sf::err().rdbuf(sfErrs.rdbuf());
        std::ofstream gameLog(".log");
        std::cout.rdbuf(gameLog.rdbuf());

        LoadData data;
        data.loaded = false;
        data.game = NULL;
        data.disp = NULL;
        int i = 0;

        try
        {
            data.wnd = new RenderWindow(VideoMode(600, 500), "CarGame loading...", Style::None);

            sf::Thread loadingThread(loadGame,&data);
            loadingThread.launch();

            data.mainLoopRunning = true;

            while(data.mainLoopRunning)
            {
                i = mainLoop(data);
                if(i != -1)
                    return i;
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
    int mainLoop(LoadData& data)
    {
        if(data.loaded)
        {
            try
            {
                bool updateDebugStats = data.game->mainTickCount % 6 == 0;
                if (!data.game->isRunning())
                    data.mainLoopRunning = false;

                data.clock.restart();
                data.eventClock.restart();

                if(updateDebugStats) data.game->times.timeGui = Time::Zero;

                bool mouseMoveHandled = false;
                //bool guiMouseMoveHandler = false;

                while(data.wnd->pollEvent(data.ev1))
                {
                    if(data.ev1.type != Event::MouseMoved || !mouseMoveHandled)
                    {
                        data.game->runEventHandler(data.ev1);
                        mouseMoveHandled = true;
                    }

                    // tick GUI for each event
                    data.guiClock.restart();
                    data.game->handleEvent(data.ev1); // run CGUI handler
                    if (updateDebugStats) data.game->times.timeGui += data.guiClock.getElapsedTime();
                }
                if (updateDebugStats) data.game->times.timeEvent = data.eventClock.getElapsedTime();

                data.tickClock.restart();
                loop(data.game);
                if (updateDebugStats) data.game->times.timeTick = data.tickClock.getElapsedTime();

                data.renderClock.restart();
                data.disp->display();
                if (updateDebugStats) data.game->times.timeRender = data.renderClock.getElapsedTime();

                if (updateDebugStats) data.game->tickTime = data.clock.getElapsedTime();

                sf::Uint64 l = data.clock.getElapsedTime().asMicroseconds();
                if(l > 16660 && (data.lastWarningClock.getElapsedTime().asSeconds() > 15.f || l > 40000))
                {
                    cout << "main: Tick took " << l << endl;
                    data.lastWarningClock.restart();
                }

                Time waitTime = microseconds(15000) - data.clock.getElapsedTime();
                //while(clock.getElapsedTime().asMicroseconds() < 16660) {} // 60 ticks/s, max framerate
                sleep(waitTime);

                if (updateDebugStats) data.game->realTickTime = data.clock.getElapsedTime();
                if (updateDebugStats) data.game->times.timeWait = waitTime;
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
            while (data.wnd->pollEvent(data.ev1))
            {
                if (data.ev1.type == Event::Closed || (data.ev1.type == Event::KeyPressed && data.ev1.key.code == Keyboard::Escape))
                    return 0;
            }
            GameDisplay::drawLoading(data.wnd);
        }
        return -1;
    }
    void reloadAllResources()
    {
        GameDisplay::instance->reload();
        Game::instance->sound.reload();
    }
}
