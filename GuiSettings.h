#ifndef GUISETTINGS_H
#define GUISETTINGS_H

#include "Gui.h"
#include "ButtonToggle.h"

class GuiSettings : public Gui
{
public:
    void onDraw(sf::RenderWindow& wnd);
    void onClick(long button);
    void onLoad();
	void onDialogFinished(Gui* dialog, int callId);

protected:

private:
    Button bDone;
    Button bResetHS;
    Button bRefreshGD;
    ButtonToggle bVerticalSync;
    Button bLanguage;
    //Button bTFM;
};

#endif // GUISETTINGS_H

