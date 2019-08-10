#pragma once

#include <CG/CG.h>
using namespace cg;

class GuiSettings : public Gui
{
public:
    void onDraw(sf::RenderWindow& wnd);
    void onClick(int button);
    void onLoad();
	void onDialogFinished(Gui* dialog, int callId);
	void onResize();

protected:

private:
    Button bDone;
    Button bResetHS;
    Button bRefreshGD;
    ButtonToggle bVerticalSync;
    Button bLanguage;
    Slider sVolume;
    //Button bTFM;
};

