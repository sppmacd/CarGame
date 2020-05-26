#include "API.hpp"

#include <cargame/Game.h>
#include <cargame/GameDisplay.h>
//#include <cargame/ModuleManager.hpp>

extern "C"
{
void cgApiInit(GameLoader* loader)
{
    Game::instance = loader->game;
    GameDisplay::instance = loader->disp;
    //ModuleManager::instance = loader->moduleManager;
}
}
