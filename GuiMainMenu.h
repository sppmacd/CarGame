#ifndef GUIMAINMENU_H
#define GUIMAINMENU_H

#include "Gui.h"


class GuiMainMenu : public Gui
{
public:
    static void draw(sf::RenderWindow* wnd);
    static void onButtonClicked(long button);
    static void onLoad();

protected:

private:
    static Button b1;
    static Button b2;
    static Button b3; //settings
};

#endif // GUIMAINMENU_H

