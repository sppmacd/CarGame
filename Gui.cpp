#include "Gui.h"
#include "GameDisplay.h"
#include "GameSound.h"
#include <cmath>
#include "Game.h"

Gui::Gui()
{
    currentDialog = nullptr;
    parent = nullptr;
}

Gui::~Gui()
{
    closeDialog(0);
}

void Gui::onDraw(RenderWindow& wnd)
{
    if(isDialogRunning())
        currentDialog->onDraw(wnd);
}

Button Gui::onMouseClick(Vector2f pos)
{
    if(!isDialogRunning())
    {
        for(Button* button: buttons)
        {
            if(button->isClicked(pos))////////////////////////////////////ES
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
    if(!isDialogRunning())
    {
        for(Button* button: buttons)
        {
            if(button->isClicked(pos))/////////////////HERE MOUSE////////////////////////////
            {
                button->isMouseOver = true;
            }
            else
            {/////////////////////////////////////////////////////////////////////////////////////////
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
        if(buttons[i]->id == widget.id)
        {
            buttons.erase(buttons.begin() + i);
            return;
        }
    }
}

void Gui::runDialog(Gui* dialog, int callId)
{
    if(isDialogRunning())
    {
        currentDialog->closeDialog(0);
    }

    currentDialog = dialog;
    currentDialog->parent = this;
    dialogCallId = callId;
    currentDialog->onLoad();
}

void Gui::close(int returnValue)
{
    if(parent)
    {
        parent->closeDialog(returnValue);
    }
    else
    {
        Game::instance->exit(0);
    }
}

void Gui::onButton(int buttonId)
{
    if(isDialogRunning())
    {
        currentDialog->onButton(buttonId);
    }
    else
    {
        this->onClick(buttonId);
    }
}

void Gui::closeDialog(int returnValue)
{
    if(isDialogRunning())
    {
        dialogReturnValue = returnValue;
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

Text Gui::drawString(string tx, int height, Vector2f pos, Text::Style style)
{
    return GameDisplay::instance->drawString(tx, height, pos, style);
}
