#pragma once

#include <CG/CG.h>
#include "UpdateChecker.hpp"

using namespace cg;

class GuiUpdates: public Gui
{
    UpdateChecker* updateChecker;
    sf::Thread updateThread;
    int animTick;
public:
    GuiUpdates(UpdateChecker* checker);
    void onLoad();
    void onClose();
    void onDialogFinished(Gui* dialog, int callId);
    void onDraw(RenderWindow& wnd);
};
