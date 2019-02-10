#ifndef GUISETTINGS_H
#define GUISETTINGS_H

#include "Gui.h"
#include "ButtonToggle.h"

class GuiSettings : public Gui
{
public:
    void onDraw(sf::RenderWindow* wnd);
    void onClick(long button);
    void onLoad();
	void onDialogFinished(int dialogId, int returnValue);

protected:

private:
    Button bDone;
    Button bResetHS;
    Button bRefreshGD;
    ButtonToggle bVerticalSync;
    Button bTFM;
};

#endif // GUISETTINGS_H

