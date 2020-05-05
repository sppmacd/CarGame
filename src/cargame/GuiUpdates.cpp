#include "GuiUpdates.hpp"
#include "Game.h"
#include <cmath>

GuiUpdates::GuiUpdates(UpdateChecker* checker): updateChecker(checker), updateThread(UpdateChecker::makeUpdate, updateChecker)
{
    animTick = 0;
}
void GuiUpdates::onLoad()
{
    runDialog(new GuiYesNo(Game::instance->translation.get("gui.updates.found", {updateChecker->getVersion()})), 0);
}
void GuiUpdates::onClose()
{
    Game::instance->exit(0);
}
void GuiUpdates::onDialogFinished(Gui* dialog, int callId)
{
    if(dialogReturnValue == 1)
    {
        updateThread.launch();
    }
}
void GuiUpdates::onDraw(RenderWindow& wnd)
{
    if(updateChecker->updateMade)
        close(0);

    RectangleShape rs(Vector2f(20.f, 20.f));
    rs.setRotation(animTick * 6.f);
    rs.setScale(sin(animTick) + 2.f, sin(animTick) + 2.f);
    wnd.draw(rs);
    animTick++;
}

