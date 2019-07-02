#ifndef GUIMAINMENU_H
#define GUIMAINMENU_H

#include "Gui.h"
#include "ButtonCircle.h"

using namespace sf;

class GuiMainMenu : public Gui
{
public:
    void onDraw(sf::RenderWindow& wnd);
    void onClick(long button);
    void onLoad();

protected:

private:
    ButtonCircle bStart;
    ButtonCircle bQuit;
    ButtonCircle bSettings; //settings
};

#endif // GUIMAINMENU_H

