#ifndef GUIGAMEOVER_H
#define GUIGAMEOVER_H

#include "Gui.h"


class GuiGameOver : public Gui
{
public:
    void onDraw(RenderWindow& wnd);
    void onClick(long buttonId);
    void onLoad();

protected:

private:
    Button b1;
    Button bMainMenu;
};

#endif // GUIGAMEOVER_H
