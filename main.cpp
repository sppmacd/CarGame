#include <SFML/Graphics.hpp>
#include "Game.h"
#include "GameDisplay.h"
#include "Game.h"
#include "GameSound.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>


using namespace std;

void loop(Game* game)
{
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

	cout << "main: Starting CarGame v0.0.4..." << endl;
	ld->disp = new GameDisplay(ld->wnd);
    srand(time(NULL));
    //ld->wnd->setVerticalSyncEnabled(true); //!!!

    cout << "main: Loading game engine..." << endl;

    GameDisplay::loadingStr = "Loading game engine...";
    
    ld->game = new Game;
    //GameSound gamesound;
	ld->loaded = true;

	loadTime.restart();

	cout << "main: Loading took " << loadTime.getElapsedTime().asMilliseconds() << "ms." << endl;
}

int main()
{
	LoadData data;
	data.loaded = false;
	data.wnd = new RenderWindow(sf::VideoMode::getFullscreenModes()[0], "Car Game v0.0.5", sf::Style::Fullscreen);
	data.wnd->setVerticalSyncEnabled(true);
	
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
			if (!data.game->isRunning())
				mainLoopRunning = false;

			clock.restart();
			eventClock.restart();

			data.game->times.timeGui = Time::Zero;

			while(data.wnd->pollEvent(ev1))
			{
				if(ev1.type == sf::Event::Closed)
				{
					data.game->exit(0);
				}

				else if(ev1.type == sf::Event::MouseButtonReleased && ev1.mouseButton.button == sf::Mouse::Left)
				{
					data.game->wasReleased = true;
				}

				else if(ev1.type == sf::Event::MouseButtonReleased && ev1.mouseButton.button == sf::Mouse::Right)
				{
					if(data.game->powerCooldown <= 0 && data.game->powerTime <= 0 && data.game->powers[data.game->getCurrentPower()] > 0)
						data.game->isPowerUsed = true;
				}

				else if(ev1.type == sf::Event::KeyPressed && ev1.key.code == sf::Keyboard::Space)
				{
					if(data.game->tutorialStep == 6)
					{
						data.game->tutorialStep = 0;
						data.game->pause(false);
					}
				}

				else if(data.game->isGameOver() && ev1.type == sf::Event::KeyPressed && ev1.key.code == sf::Keyboard::Return)
				{
					data.game->loadGame();
				}

				else if(ev1.type == sf::Event::KeyPressed && ev1.key.code == sf::Keyboard::Escape && !data.game->isGameOver() && !data.game->paused())
				{
					cout << "main: Pausing data.game->.." << endl;
					data.game->displayGui(0);
					data.game->pause(true);
				}

				else if(ev1.type == sf::Event::KeyPressed && ev1.key.code == sf::Keyboard::F11)
				{
					data.game->toggleFullscreen();
				}

				else if(ev1.type == sf::Event::KeyPressed && ev1.key.code == sf::Keyboard::F3 && ev1.key.shift)
				{
					data.game->debug = !data.game->debug;
				}

				else if(ev1.type == sf::Event::MouseWheelScrolled)
				{
					data.game->wheelEvent(ev1.mouseWheelScroll);
				}

				else if (ev1.type == sf::Event::KeyPressed && ev1.key.code == sf::Keyboard::Escape && data.game->displayedGui == 0) //close ingame GUI on Esc (0.0.5)
				{
					data.game->closeGui();
				}

				// tick GUI for each event

				if (data.game->guiCooldown <= 0 && data.game->isGuiLoaded)
				{
					guiClock.restart();
					data.game->tickGui(ev1);
					data.game->times.timeGui += guiClock.getElapsedTime();
				}
			}
			data.game->times.timeEvent = eventClock.getElapsedTime();

			tickClock.restart();
			loop(data.game);
			data.game->times.timeTick = tickClock.getElapsedTime();

			renderClock.restart();
			data.disp->display();
			data.game->times.timeRender = renderClock.getElapsedTime();

			data.game->tickTime = clock.getElapsedTime();

			sf::Uint64 l = clock.getElapsedTime().asMicroseconds();

			if(l > 10000 && data.game->guiCooldown == 0)
			{
				cout << "main: Tick took " << l << endl;
			}

			waitClock.restart();
			while(clock.getElapsedTime().asMicroseconds() < 16660) {} // 60 ticks/s, max framerate

			data.game->realTickTime = clock.getElapsedTime();
			data.game->times.timeWait = waitClock.getElapsedTime();
		}
		else
		{
			data.disp->drawLoading(data.wnd);
		}
    }

    cout << "main: Closing CarGame v0.0.4..." << endl;

    GameDisplay::drawLoadingProgress("Closing...", data.wnd);

    data.game->savePlayerData();
    int i = data.game->retVal;

    data.wnd->close();

	delete data.disp;
	delete data.game;
	delete data.wnd;

    return i;
}
