#pragma once

#include "Gui.h"

class GuiLanguge : public Gui
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
    void onClick(long buttonId);
};
