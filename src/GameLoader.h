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

    // Called on load once
    void loadGame(LoadData* ld);

    // Call game logic loop.
    void loop(Game* game);

    // Run the game. Returns return value of application.
    int run();

    // Run main loop iteration.
    int mainLoop(LoadData& data);

    // Reload all resources. Called by loadGame and GuiSettings.
    void reloadAllResources();
}
