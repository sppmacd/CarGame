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
    Button b1;
    Button b2;
    Button b3; //settings
};

#endif // GUIMAINMENU_H

