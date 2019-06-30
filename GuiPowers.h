#ifndef GUIPOWERS_H
#define GUIPOWERS_H

#include "Gui.h"

class GuiPowers : public Gui
{
public:
    void onDraw(sf::RenderWindow& wnd);
    void onClick(long button);
    void onLoad();

protected:

private:
    Button bPower1;
    Button bPower2;
    Button bReturn; //go to map selection
    int cooldown;
};

#endif // GUIPOWERS_H


