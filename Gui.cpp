#include "Gui.h"
#include "GameDisplay.h"
#include "GameSound.h"
#include <cmath>
#include "Game.h"

vector<Button*> Gui::buttons;
map<int, GuiData> Gui::registry;
vector<Button*>Gui::dialogButtons;
bool Gui::runningDialog;
int Gui::dialogId;
int Gui::dialogRv;

void Gui::addButton(Button& button)
{
    buttons.push_back(&button);
}

void Gui::addButtonToDialog(Button & button)
{
	dialogButtons.push_back(&button);
}

void Gui::runDialog(int id)
{
	runningDialog = true;
	dialogId = id;
	dialogRv = -1;

	GuiData handler = Gui::findHandlerByID(id);
	handler.load();
}

int Gui::getDialogReturnValue()
{
	int rv = dialogRv;
	dialogRv = -2; //not yet started
	return rv;
}

void Gui::drawDialog(sf::RenderWindow * wnd)
{
	GuiData handler = Gui::findHandlerByID(dialogId);
	handler.draw(wnd);
}

void Gui::onLoad()
{
	dialogRv = -2; //not yet started
}

Button Gui::onMouseClick(sf::Vector2f vec)
{
	if (!runningDialog)
	{
		for (unsigned int i = 0; i < buttons.size(); i++)
		{
			if (sf::Rect<float>(buttons[i]->getPos(), buttons[i]->getSize()).contains(vec) && buttons[i]->enabled)
			{
				buttons[i]->onClick();
				return *buttons[i];
			}
		}
	}
	else
	{
		for (unsigned int i = 0; i < dialogButtons.size(); i++)
		{
			if (sf::Rect<float>(dialogButtons[i]->getPos(), dialogButtons[i]->getSize()).contains(vec) && dialogButtons[i]->enabled)
			{
				dialogButtons[i]->onClick();
				return *dialogButtons[i];
			}
		}
	}

    return Button();
}

void Gui::onClose()
{
    if(!buttons.empty())
		buttons.clear();

	if (!dialogButtons.empty())
		dialogButtons.clear();
}

void Gui::onMouseMove(sf::Vector2f vec)
{
	if (!runningDialog)
	{
		for (unsigned int i = 0; i < buttons.size(); i++)
		{
			if (sf::Rect<float>(buttons[i]->getPos(), buttons[i]->getSize()).contains(vec))
			{
				buttons[i]->isMouseOver = true;
			}
			else
			{
				buttons[i]->isMouseOver = false;
			}
		}
	}
	else
	{
		for (unsigned int i = 0; i < dialogButtons.size(); i++)
		{
			if (sf::Rect<float>(dialogButtons[i]->getPos(), dialogButtons[i]->getSize()).contains(vec))
			{
				dialogButtons[i]->isMouseOver = true;
			}
			else
			{
				dialogButtons[i]->isMouseOver = false;
			}
		}
	}
}

void Gui::onButtonClicked(long button)
{

}

void Gui::registerGuiHandlers(int guiId, GuiData gui)
{
	registry.insert(make_pair(guiId, gui));
}

void Gui::clearRegistry()
{
	registry.clear();
}

void Gui::drawNFG(sf::RenderWindow* wnd)
{
	Gui::drawGui(wnd);

	sf::Text t = drawString("Sorry, we can't find this GUI ID. Please report this problem to game developers.", 30, sf::Vector2f(wnd->getSize().x / 2, wnd->getSize().y / 2), sf::Text::Bold);
	t.setOrigin(t.getLocalBounds().width/2, t.getLocalBounds().height/2);
	wnd->draw(t);
}

Button Gui::mouseNFG(sf::Vector2f pos)
{
	Game::instance->displayGui(2);
	return Button();
}

const GuiData Gui::findHandlerByID(int id)
{
	GuiData gd;

	try
	{
		 gd = registry.find(id)->second;
	}
	catch (out_of_range)
	{
		cout << "Cannot find GUI with id " << id << endl;
		gd = GuiData( Gui::drawNFG, Gui::mouseNFG, Gui::onMouseMove, Gui::onButtonClicked, Gui::onLoad, Gui::onClose );
		return gd;
	}
	return gd;
}

void Gui::drawGui(sf::RenderWindow* wnd)
{
    sf::RectangleShape rect((sf::Vector2f) wnd->getSize());
    rect.setFillColor(sf::Color(25, 20, 20, 200));
    wnd->draw(rect);
}

sf::Text Gui::drawString(string tx, int height, sf::Vector2f pos, sf::Text::Style style)
{
    return GameDisplay::instance->drawString(tx, height, pos, style);
}

void Gui::setDialogReturnValue(int rv)
{
	dialogRv = rv;
	runningDialog = false;
	dialogId = -1;

	if (!dialogButtons.empty())
		dialogButtons.clear();
}

GuiData::GuiData(GUIDrawFunc drawFunc1, GUIMouseClickHandler mouseClickHandler1, GUIMouseMoveHandler mouseMoveHandler1, GUIButtonHandler buttonHandler1, GUILoadHandler loadHandler1, GUICloseHandler closeHandler1) :
	draw(drawFunc1),
		onMouseClick(mouseClickHandler1),
		onMouseMove(mouseMoveHandler1),
		onButtonClick(buttonHandler1),
		load(loadHandler1),
		close(closeHandler1) {}
