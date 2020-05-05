#include "ButtonCircle.h"
#include <cmath>
#include "GameDisplay.h"

using namespace std;

ButtonCircle::ButtonCircle(Gui* gui, float radius, Vector2f pos, string imageName, int id) : ButtonImage(gui, imageName, Vector2f(radius,radius), pos, "", id)
{
    bRadius = radius;
}
void ButtonCircle::setRadius(float radius)
{
    bRadius = radius;
}
void ButtonCircle::draw(RenderWindow& wnd)
{
    sf::CircleShape cs(bRadius);
	cs.setPosition(this->getPosition());
	cs.setOrigin(bRadius, bRadius);
	cs.setFillColor(sf::Color(119, 103, 50));
	cs.setOutlineThickness(2.2f);
	cs.setOutlineColor(this->bColor);

	if (this->isMouseOver && this->enabled)
		cs.setFillColor(sf::Color(240, 206, 100));
	if (!this->enabled)
		cs.setFillColor(sf::Color(60, 51, 25));

	wnd.draw(cs);

	Sprite spr(*this->img);
	spr.setPosition(this->getPosition());
	spr.setOrigin(Vector2f(this->img->getSize() / 2U));
	if (!enabled) spr.setColor(Color(128, 128, 128));
	Vector2f a = Vector2f(this->getSize().x / Vector2f(this->img->getSize()).x, this->getSize().y / Vector2f(this->img->getSize()).y);
	spr.setScale((isMouseOver && enabled) ? Vector2f(0.97f*a.x, 0.97f*a.y) : Vector2f(0.93f*a.x, 0.93f*a.y));
	wnd.draw(spr);
}
bool ButtonCircle::isClicked(Vector2f pos)
{
    Vector2f pos2 = pos - this->getPosition();
    return sqrt(pos2.x*pos2.x+pos2.y*pos2.y) <= bRadius;
}
