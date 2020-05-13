#ifndef WINDOWSGAMELOADER_HPP
#define WINDOWSGAMELOADER_HPP

#include <string>

#include <cargame/GameLoader.hpp>

// GameLoader for desktop OS's.
class DesktopGameLoader : public GameLoader
{
public:
    DesktopGameLoader();
};

// GameLoader for Windows.
class WindowsGameLoader : public DesktopGameLoader
{
public:
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

