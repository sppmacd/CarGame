#include "ButtonImage.h"
#include "GameDisplay.h"

ButtonImage::ButtonImage(string imgName, sf::Vector2f size, sf::Vector2f pos, string text, int id) : Button(size,pos,text,id)
{
	this->img = &GameDisplay::instance->texturesByName.find(imgName)->second;
	this->bText = imgName;
}

ButtonImage::ButtonImage() : ButtonImage("stat/mpl", Vector2f(0, 0), Vector2f(100, 30), "ButtonImage", -1) {}

void ButtonImage::draw(RenderWindow* wnd)
{
	sf::RectangleShape rect(this->getSize());
	rect.setPosition(this->getPos());
	rect.setFillColor(sf::Color(119, 103, 50));
	rect.setOutlineThickness(2.2f);
	rect.setOutlineColor(this->bColor);

	if (this->isMouseOver && this->enabled)
		rect.setFillColor(sf::Color(240, 206, 100));
	if (!this->enabled)
		rect.setFillColor(sf::Color(60, 51, 25));

	wnd->draw(rect);

	Sprite spr(*this->img);
	spr.setPosition(this->bPos);
	spr.setOrigin(this->bSize / 2.f);
	wnd->draw(spr);
}