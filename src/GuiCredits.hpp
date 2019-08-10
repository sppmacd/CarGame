#pragma once

#include <CG/CG.h>
using namespace cg;

class GuiCredits: public Gui
{
public:
    Button bOk;
    virtual void onClick(int id);
    virtual void onResize();
    virtual void onLoad();
    virtual void onDraw(RenderWindow& wnd);
};
