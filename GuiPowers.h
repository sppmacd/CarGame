#ifndef GUIPOWERS_H
#define GUIPOWERS_H

#include "Gui.h"

class GuiPowers : public Gui
{
public:
    static void draw(sf::RenderWindow* wnd);
    static void onButtonClicked(long button);
    static void onLoad();

protected:

private:
    static Button bPower1;
    static Button bPower2;
    static Button bReturn; //go to map selection
    static int cooldown;
};

#endif // GUIPOWERS_H


