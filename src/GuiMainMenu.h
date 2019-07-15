#pragma once

#include <CG/CG.h>
using namespace cg;

#include "ButtonCircle.h"

using namespace sf;

class GuiMainMenu : public Gui
{
public:
    void onDraw(sf::RenderWindow& wnd);
    void onClick(int button);
    void onLoad();
    void onResize();

protected:

private:
    ButtonCircle bStart;
    ButtonCircle bQuit;
    ButtonCircle bSettings;
    Slider sTest;
};
