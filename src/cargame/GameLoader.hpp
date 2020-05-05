#ifndef GAMELOADER_HPP
#define GAMELOADER_HPP

#include "ArgMap.hpp"
#include "Game.h"
#include "GameDisplay.h"

#include <SFML/Graphics.hpp>

class GameLoader
{
public:
    // from 0.2 LoadData
	sf::RenderWindow* wnd;
	Game* game;
	GameDisplay* disp;
	bool loaded;
	ArgMap* argmap;

    // Thread entry for game loading.
	void loadGame();

	// Game loop.
	void loop(Game* game);
};

#endif // GAMELOADER_HPP

