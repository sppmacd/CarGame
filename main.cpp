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
	sf::Clock loadTime;

	cout << "main: Starting CarDestroyer v0.1..." << endl;

	GameDisplay::loadingStr = "Loading game engine...";
	ld->game = new Game;
	ld->disp = new GameDisplay(ld->wnd);

    srand(time(NULL));

    cout << "main: Loading game engine..." << endl;

    //GameSound gamesound;

	ld->loaded = true;
	cout << "main: Loading took " << loadTime.getElapsedTime().asMilliseconds() << "ms." << endl;
}

int main()
{
	LoadData data;
	data.loaded = false;
	data.wnd = new RenderWindow(sf::VideoMode::getFullscreenModes()[0], "Car Destroyer v0.1", Style::Fullscreen);
	data.wnd->setVerticalSyncEnabled(true);
	data.game = NULL;
	data.disp = NULL;

	sf::Thread loadingThread(loadGame,&data);

	loadingThread.launch();
	sf::Clock clock;
	sf::Clock eventClock;
	sf::Clock guiClock;
	sf::Clock tickClock;
	sf::Clock renderClock;
	sf::Clock waitClock;
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
			while(data.wnd->pollEvent(ev1))
			{
			    if(ev1.type != Event::MouseMoved || mouseMoveHandled)
                {
                    data.game->runEventHandler(ev1);
                    mouseMoveHandled = true;
                }
				// tick GUI for each event

				if (data.game->isGuiLoaded && (ev1.type == Event::MouseMoved || ev1.type == Event::MouseButtonReleased || ev1.type == Event::KeyPressed))
                {
                    guiClock.restart();
                    data.game->tickGui(ev1);
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

			if(l > 16660 && data.game->guiCooldown == 0)
			{
				cout << "main: Tick took " << l << endl;
			}

			waitClock.restart();
			while(clock.getElapsedTime().asMicroseconds() < 16660) {} // 60 ticks/s, max framerate

			if (updateDebugStats) data.game->realTickTime = clock.getElapsedTime();
			if (updateDebugStats) data.game->times.timeWait = waitClock.getElapsedTime();
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

    cout << "main: Closing CarDestroyer v0.1..." << endl;

    GameDisplay::drawLoadingProgress("Closing...", data.wnd);

	int i;

	if (data.loaded)
	{
		data.game->savePlayerData();
		i = data.game->retVal;
	}
	else
		i = 0;

	delete data.disp;
	delete data.game;

	data.wnd->close();
	delete data.wnd;

    return i;
}
