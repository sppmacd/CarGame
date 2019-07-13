#pragma once

#include <CG/CG.h>
using namespace cg;

class GuiLanguage : public Gui
{
    vector<Button> bLangButtons;
    Button bCancel;
    Button bDone;
    Button bUp;
    Button bDown;
    int current;
public:
    void onLoad();
    void onDraw(RenderWindow& wnd);
    void onClick(int buttonId);
    void onResize();
};
