#pragma once

#include <CG/CG.h>
using namespace cg;

class GuiIngame : public Gui
{
public:
    void onDraw(sf::RenderWindow& wnd);
    void onClick(int button);
    void onLoad();
    void onKeyboardEvent(Keyboard::Key key, bool released, bool alt, bool shift, bool ctrl, bool sys);

protected:

private:
    Button b1;
    Button b2;
    Button bQuit;
};
