#include "WindowsGameLoader.hpp"

#include <SFML/Graphics.hpp>
#include <windows.h>
#include <cargame/EventHandler.h>
#include <cargame/ArgMap.hpp>
#include "WindowsModuleManager.hpp"

DesktopGameLoader::DesktopGameLoader() : GameLoader() {}

WindowsGameLoader::WindowsGameLoader()
                : DesktopGameLoader()
                , loadingThread(GameLoader::loadGame, (GameLoader*)this) {}

void WindowsGameLoader::preInit()
{
    // redirect SFML error output to null if not debug mode
    if(!argmap->a_debug) sf::err().rdbuf(NULL);

    // create modmanager
    modManager = new WindowsModuleManager;
    modManager->setCurrent("root");
}
void WindowsGameLoader::createLoadingWnd()
{
    wnd = new sf::RenderWindow(VideoMode(600, 500), "CarGame loading...", Style::None);
}
void WindowsGameLoader::startLoadingThread()
{
    //sf::Thread loadingThread(WindowsGameLoader::loadGame, (WindowsGameLoader*)this);
    loadingThread.launch();

    //wnd->setActive(true);
}
void WindowsGameLoader::registerEventHandlers()
{
	game->addEventHandler(sf::Event::Closed, EventHandlers::onClose);
	game->addEventHandler(sf::Event::MouseButtonReleased, EventHandlers::onMouseButtonReleased);
	game->addEventHandler(sf::Event::KeyPressed, EventHandlers::onKeyPressed);
	game->addEventHandler(sf::Event::MouseWheelScrolled, EventHandlers::onMouseWheelScrolled);
}
void WindowsGameLoader::checkEvents()
{
    sf::Event ev1;
    bool mouseMoveHandled = false;
    sf::Clock guiClock;
    bool updateDebugStats = true;

    while(wnd->pollEvent(ev1))
    {
        // Call event handler
        if(ev1.type != Event::MouseMoved || !mouseMoveHandled)
        {
            //DebugLogger::logDbg("Event:" + std::to_string(ev1.type));
            game->runEventHandler(ev1);
            mouseMoveHandled = true;
        }

        // Tick GUI for each event
        guiClock.restart();
        game->handleEvent(ev1); // run CGUI handler
        if (updateDebugStats) game->times.timeGui += guiClock.getElapsedTime();
    }
}
bool WindowsGameLoader::loadingCheckEvents()
{
    sf::Event ev1;
    while(wnd->pollEvent(ev1))
    {
        if(ev1.type == Event::Closed || (ev1.type == Event::KeyPressed && ev1.key.code == Keyboard::Escape))
            return false;
    }
    return true;
}

int WindowsGameLoader::levelToColor(std::string lvl)
{
    if(lvl == "INFO")
        return 0x07;
    else if(lvl == "WARN")
        return 0x06;
    else if(lvl == "ERROR")
        return 0x0C;
    else if(lvl == "FATAL")
        return 0xCF;
    else if(lvl == "LAG")
        return 0x0B;
    else if(lvl == "DEBUG")
        return 0x08;
    else if(lvl == "EVENT")
        return 0x0D;
    else if(lvl == "IMP")
        return 0x01;
    else if(lvl == "TEXT")
        return 0x0A;
    else
        return 0x07;
}

void WindowsGameLoader::consoleColor(std::string level)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, levelToColor(level));
}
