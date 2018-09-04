#ifndef GUIGAMEOVER_H
#define GUIGAMEOVER_H

#include "Gui.h"


class GuiGameOver : public Gui
{
public:
    static void draw(sf::RenderWindow* wnd);
    static void onButtonClicked(long button);
    static void onLoad();

protected:

private:
    static Button b1;
    static Button bMainMenu;
};

#endif // GUIGAMEOVER_H
