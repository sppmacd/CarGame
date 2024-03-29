#ifndef WINDOWSGAMELOADER_HPP
#define WINDOWSGAMELOADER_HPP

#include <string>

#include <cargame/GameLoader.hpp>

#include <SFML/System.hpp>

// GameLoader for Windows.
class WindowsGameLoader : public DesktopGameLoader
{
public:
    sf::Thread loadingThread;

    WindowsGameLoader();

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

#endif // WINDOWSGAMELOADER_HPP

