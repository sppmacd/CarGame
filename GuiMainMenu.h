#ifndef GUIMAINMENU_H
#define GUIMAINMENU_H

#include "Gui.h"

class GuiMainMenu : public Gui
{
public:
    void onDraw(sf::RenderWindow& wnd);
    void onClick(long button);
    void onLoad();

protected:

private:
    Button bStart;
    Button bQuit;
    Button bSettings; //settings
};

#endif // GUIMAINMENU_H

