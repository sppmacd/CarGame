#ifndef GUISETTINGS_H
#define GUISETTINGS_H

#include "Gui.h"
#include "ButtonToggle.h"

class GuiSettings : public Gui
{
public:
    static void draw(sf::RenderWindow* wnd);
    static void onButtonClicked(long button);
    static void onLoad();
	static void onDialogFinished(int id, int rv);

protected:

private:
    static Button bDone;
    static Button bResetHS;
    static Button bRefreshGD;
    static ButtonToggle bVerticalSync;
    static Button bTFM;
};

#endif // GUISETTINGS_H

