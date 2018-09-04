#ifndef GUIINGAME_H
#define GUIINGAME_H

#include "Gui.h"


class GuiIngame : public Gui
{
public:
	GuiIngame() {};
	virtual ~GuiIngame() {};

    static void draw(sf::RenderWindow* wnd);
    static void onButtonClicked(long button);
    static void onLoad();

protected:

private:
    static Button b1;
    static Button b2;
    static Button bQuit;
};

#endif // GUIINGAME_H
