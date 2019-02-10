#include "Gui.h"
#include "GameDisplay.h"
#include "GameSound.h"
#include <cmath>
#include "Game.h"

void Gui::onDraw(RenderWindow& wnd)
{
    if(isDialogRunning())
        currentDialog->onDraw(wnd);
}

Button Gui::onMouseClick(Vector2f pos)
{
    if(!runningDialog)
    {
        for(Button* button: buttons)
        {
            FloatRect buttonRect(button->getPos(), button->getSize());
            if(buttonRect.contains(pos))
            {
                button->onClick();
                return *button;
            }
        }
    }
    else
    {
        return currentDialog->onMouseClick(pos);
    }
    return Button();
}

void Gui::onMouseMove(Vector2f pos)
{
    if(!runningDialog)
    {
        for(Button* button: buttons)
        {
            FloatRect buttonRect(button->getPos(), button->getSize());
            if(buttonRect.contains(pos))
            {
                button->isMouseOver = true;
            }
            else
            {
                button->isMouseOver = false;
            }
        }
    }
    else
    {
        currentDialog->onMouseMove(pos);
    }
}

void Gui::onClick(long buttonId)
{
    cout << "You'll never see this message! \n\tButton (ID " << buttonId << ")" << endl;
}

void Gui::addButton(Button& widget)
{
    buttons.push_back(&widget);
}

void Gui::removeButton(Button& widget)
{
    for(unsigned int i = 0; i < buttons.size(); i++)
    {
        if(buttons[i] == widget);
        {
            buttons.erase(buttons.begin() + i);
            return;
        }
    }
}

void Gui::runDialog(Gui* dialog, int callId)
{
    currentDialog->closeDialog();
    currentDialog = dialog;
    dialogCallId = callId
    currentDialog->onLoad();
}

void Gui::closeDialog()
{
    if(isDialogRunning())
    {
        currentDialog->onClose();
        this->onDialogFinished(currentDialog, dialogCallId);
        delete currentDialog;
        currentDialog = nullptr;
    }
}

void Gui::onLoad() {}

void Gui::onClose() {}

void Gui::onKeyboard(Keyboard::Key key) {}

void Gui::onDialogFinished(Gui* dialog, int callId) {}

bool Gui::isDialogRunning()
{
    return currentDialog != nullptr;
}

Text Gui::drawString(string tx, int height, Vector2f pos, Text::Style style = Text::Regular)
{
    return GameDisplay::drawString(tx, height, pos, style);
}

void Gui::setDialogReturnValue(int rv)
{
    dialogReturnValue = rv;
}
