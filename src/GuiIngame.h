#ifndef GUIINGAME_H
#define GUIINGAME_H

#include "Gui.h"


class GuiIngame : public Gui
{
public:
    void onDraw(sf::RenderWindow& wnd);
    void onClick(long button);
    void onLoad();

protected:

private:
    Button b1;
    Button b2;
    Button bQuit;
};

#endif // GUIINGAME_H
