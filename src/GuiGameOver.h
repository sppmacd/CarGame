#pragma once

#include <CG/CG.h>
using namespace cg;

class GuiGameOver : public Gui
{
public:
    void onDraw(RenderWindow& wnd);
    void onClick(int buttonId);
    void onLoad();

protected:

private:
    Button b1;
    Button bMainMenu;
};
