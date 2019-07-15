#include <SFML/Graphics.hpp>
#include "Game.h"
#include "GameDisplay.h"
#include "GuiMainMenu.h"
#include "GuiYesNo.h"
#include "GameSound.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <exception>

using namespace std;

void loop(Game* game)
{
	if (game->mainTickCount == 0)
		// Display the main menu
		game->displayGui(new GuiMainMenu);

    if(!game->errStr.empty())
            game->displayGui(new GuiYesNo("An error occured: " + game->errStr));

    if(!game->paused())
        game->tickNormalGame();

    game->newTick();
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
        ld->disp = new GameDisplay(ld->wnd);
        ld->game = new Game;
        ld->disp->reload(); // moved from constructor to display loading screen.

        GameDisplay::loadingStr = "Loading game engine...";

        if(ld->disp->isError())
            throw runtime_error("GameDisplay loading error");

        srand(time(NULL));

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
    LoadData data;
    data.loaded = false;
    data.game = NULL;
    data.disp = NULL;
    int i = 0;

    try
    {
        data.wnd = new RenderWindow(VideoMode(600, 500), "CarGame loading...", Style::None);
        data.wnd->setVerticalSyncEnabled(true);

        sf::Thread loadingThread(loadGame,&data);

        loadingThread.launch();
        sf::Clock clock;
        sf::Clock eventClock;
        sf::Clock guiClock;
        sf::Clock tickClock;
        sf::Clock renderClock;
        sf::Event ev1;

        bool mainLoopRunning = true;

        while(mainLoopRunning)
        {
            if(data.loaded)
            {
                bool updateDebugStats = data.game->mainTickCount % 6 == 0;
                if (!data.game->isRunning())
                    mainLoopRunning = false;

                clock.restart();
                eventClock.restart();

                if(updateDebugStats) data.game->times.timeGui = Time::Zero;

                bool mouseMoveHandled = false;
                bool guiMouseMoveHandler = false;
                while(data.wnd->pollEvent(ev1))
                {
                    if(ev1.type != Event::MouseMoved || !mouseMoveHandled)
                    {
                        data.game->runEventHandler(ev1);
                        mouseMoveHandled = true;
                    }
                    // tick GUI for each event

                    if (data.game->isGuiLoaded && ((ev1.type == Event::MouseMoved && !guiMouseMoveHandler) || ev1.type == Event::MouseButtonReleased || ev1.type == Event::KeyPressed))
                    {
                        guiClock.restart();
                        data.game->tickGui(ev1);
                        guiMouseMoveHandler = true;
                        if (updateDebugStats) data.game->times.timeGui += guiClock.getElapsedTime();
                    }
                }
                if (updateDebugStats) data.game->times.timeEvent = eventClock.getElapsedTime();

                tickClock.restart();
                loop(data.game);
                if (updateDebugStats) data.game->times.timeTick = tickClock.getElapsedTime();

                renderClock.restart();
                data.disp->display();
                if (updateDebugStats) data.game->times.timeRender = renderClock.getElapsedTime();

                if (updateDebugStats) data.game->tickTime = clock.getElapsedTime();

                sf::Uint64 l = clock.getElapsedTime().asMicroseconds();

                if(l > 16660)
                {
                    cout << "main: Tick took " << l << endl;
                }

                Time waitTime = microseconds(15000) - clock.getElapsedTime();
                //while(clock.getElapsedTime().asMicroseconds() < 16660) {} // 60 ticks/s, max framerate
                sleep(waitTime);

                if (updateDebugStats) data.game->realTickTime = clock.getElapsedTime();
                if (updateDebugStats) data.game->times.timeWait = waitTime;
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

    cout << "main: Unloading resources..." << endl;
    delete data.disp;
    delete data.game;

    data.wnd->close();
    delete data.wnd;

    return i;
}
