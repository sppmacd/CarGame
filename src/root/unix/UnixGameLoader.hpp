#ifndef UNIXGAMELOADER_HPP
#define UNIXGAMELOADER_HPP

#include <string>

#include <cargame/GameLoader.hpp>

#include <SFML/System.hpp>

// GameLoader for Unix-like systems.
class UnixGameLoader : public DesktopGameLoader
{
public:
    sf::Thread loadingThread;

    UnixGameLoader();

    virtual void preInit();
    virtual void createLoadingWnd();
    virtual void startLoadingThread();
    virtual void registerEventHandlers();
    virtual void checkEvents();
    virtual bool loadingCheckEvents();

    virtual void consoleColor(std::string level);

    // Converts level string to Windows color id (as in COLOR command)
    int levelToColor(std::string lvl);
};

#endif // UNIXGAMELOADER_HPP

