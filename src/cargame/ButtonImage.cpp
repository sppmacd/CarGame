#include "ButtonImage.h"
#include "GameDisplay.h"

ButtonImage::ButtonImage(Gui* gui, String imgName, sf::Vector2f size, sf::Vector2f pos, string, int id) : Button(gui,size,pos,"",id)
{
	this->img = &GameDisplay::instance->getTexture(imgName);
	this->bText = imgName;
}

ButtonImage::ButtonImage() : ButtonImage(NULL, "stat/mpl", Vector2f(0, 0), Vector2f(100, 100), "ButtonImage", -1) {}

void ButtonImage::draw(RenderWindow& wnd)
{
	sf::RectangleShape rect(this->getSize());
	rect.setPosition(this->getPosition());
	rect.setFillColor(sf::Color(119, 103, 50));
	rect.setOutlineThickness(2.2f);
	rect.setOutlineColor(this->bColor);

	if (this->isMouseOver && this->enabled)
		rect.setFillColor(sf::Color(240, 206, 100));
	if (!this->enabled)
		rect.setFillColor(sf::Color(60, 51, 25));

	wnd.draw(rect);

	Sprite spr(*this->img);
	spr.setPosition(this->getPosition() + this->getSize() / 2.f);
	spr.setOrigin(Vector2f(this->img->getSize() / 2U));
	if (!enabled) spr.setColor(Color(128, 128, 128));
	Vector2f a = Vector2f(this->getSize().x / Vector2f(this->img->getSize()).x, this->getSize().y / Vector2f(this->img->getSize()).y);
	spr.setScale((isMouseOver && enabled) ? Vector2f(0.97f*a.x, 0.97f*a.y) : Vector2f(0.93f*a.x, 0.93f*a.y));
	wnd.draw(spr);
}
