#ifndef GUIMAPSELECT_H
#define GUIMAPSELECT_H

#include "Gui.h"

class GuiMapSelect : public Gui
{
public:
    static void draw(sf::RenderWindow* wnd);
    static void onButtonClicked(long button);
    static void onLoad();

protected:

private:
    static Button bMap1;
    static Button bMap2;
    static Button bMap3; //settings
    static Button bMap4; //settings
    static Button bMap5; //settings

    static Button bMapB1;
    static Button bMapB2;
    static Button bMapB3; //settings
    static Button bMapB4; //settings
    static Button bMapB5; //settings
    static Button bReturn;

    static Button bPowers; //unlock powers
};

#endif // GUIMAPSELECT_H

