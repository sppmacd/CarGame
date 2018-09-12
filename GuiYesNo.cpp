#include "GuiYesNo.h"
#include "GameDisplay.h"

Button GuiYesNo::bYes;
Button GuiYesNo::bNo;
string GuiYesNo::vstr;

void GuiYesNo::draw(RenderWindow* wnd)
{
	// Background 1
	RectangleShape rsx(Vector2f(GameDisplay::instance->getSize()));
	rsx.setFillColor(Color(0, 0, 0, 80));
	wnd->draw(rsx);

	//////////////////////////////////////////////
	//	|---------------------------|
	//	|       |15%|   |15%|       |
	//	|--25%--|Yes|20%| No|--25%--|
	//	|---------------------------|
	//////////////////////////////////////////////

	// Background 2
	

	// Text
	Text tx = GameDisplay::instance->drawCenteredString(vstr, 30, Vector2f(GameDisplay::instance->getSize().x / 2, 300));

	RectangleShape rsb(Vector2f(tx.getLocalBounds().width + 40.f, tx.getLocalBounds().height + 90.f));
	rsb.setOrigin(rsb.getSize() / 2.f);
	rsb.setFillColor(Color(110, 100, 65));
	rsb.setPosition(GameDisplay::instance->getSize().x / 2.f, 340);
	rsb.setOutlineColor(Color(55, 50, 33));
	rsb.setOutlineThickness(2.5f);
	wnd->draw(rsb);

	wnd->draw(tx);
	bYes.draw(wnd);
	bNo.draw(wnd);
}


void GuiYesNo::onButtonClicked(long button)
{
	if (button == 1)
		Gui::setDialogReturnValue(1);
	if (button == 0)
		Gui::setDialogReturnValue(0);
}


void GuiYesNo::onLoad()
{
	RenderWindow* wnd = GameDisplay::instance->getRenderWnd();

	// Simulate buttons' position
	Text tx = GameDisplay::instance->drawCenteredString(vstr, 30, Vector2f(GameDisplay::instance->getSize().x / 2, 300));
	tx.setOrigin(tx.getLocalBounds().width / 2, tx.getLocalBounds().height / 2);
	tx.setPosition(GameDisplay::instance->getSize().x / 2, 300);

	RectangleShape rsb(Vector2f(tx.getLocalBounds().width + 40.f, tx.getLocalBounds().height + 90.f));
	rsb.setOrigin(rsb.getSize() / 2.f);
	rsb.setFillColor(Color(110, 100, 65));
	rsb.setPosition(Vector2f(GameDisplay::instance->getSize()) / 2.f);

	bYes = Button(Vector2f(rsb.getSize().x * 0.30f, 40.f), Vector2f(rsb.getPosition().x - rsb.getSize().x *0.40f, tx.getGlobalBounds().top + tx.getGlobalBounds().height + 20.f), "Yes", true);
	bNo = Button(Vector2f(rsb.getSize().x * 0.30f, 40.f), Vector2f(rsb.getPosition().x + rsb.getSize().x *0.10f, tx.getGlobalBounds().top + tx.getGlobalBounds().height + 20.f), "No", false);

	Gui::addButtonToDialog(bYes);
	Gui::addButtonToDialog(bNo);
}
