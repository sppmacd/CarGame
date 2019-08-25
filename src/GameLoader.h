#pragma once
#include "Game.h"
#include <SFML/Graphics.hpp>
#include "GameDisplay.h"

namespace Loader
{
    struct LoadData
    {
        sf::RenderWindow* wnd;
        Game* game;
        GameDisplay* disp;
        bool loaded;
        bool mainLoopRunning;
        sf::Clock clock;
        sf::Clock eventClock;
        sf::Clock guiClock;
        sf::Clock tickClock;
        sf::Clock renderClock;
        sf::Clock lastWarningClock;
        sf::Event ev1;
    };

    void loadGame(LoadData* ld);
    void loop(Game* game);
    int run();
    int mainLoop(LoadData& data);
}
