#include "Power.h"
#include "GameDisplay.h"
#include "Game.h"

bool Power::onPowerStart()
{
	return true;
}

void Power::onPowerStop() {}

void Power::onPowerTick(int powerTick) {}

void Power::onCooldownTick(int cooldownTick) {}

void Power::onCooldownStop() {}

void Power::drawPower(RenderWindow * wnd) {}

void Power::drawPowerIdle(RenderWindow * wnd) {}

//// OIL ////

void PowerOil::onPowerStop()
{
	Game::instance->setGameSpeed(Game::instance->getGameSpeed() / 1.8f);
}

bool PowerOil::onPowerStart()
{
	Game::instance->setGameSpeed(Game::instance->getGameSpeed() * 1.8f);
	pos = Vector2f(Mouse::getPosition(*GameDisplay::instance->getRenderWnd()));
	return true;
}

Vector2f PowerOil::pos;

void PowerOil::onPowerTick(int powerTick)
{
	if (powerTick % 10 == 0)
	{
		for (Car& c : Game::instance->cars)
		{
			if(abs(c.getScreenPos().x - pos.x) < 42)
				c.makeDestroy();
		}
	}
}

void PowerOil::drawPower(RenderWindow * wnd)
{
	RectangleShape rs(Vector2f(40.f, 500.f));
	rs.setFillColor(Color(219, 201, 65, 180));
	rs.setOutlineColor(Color(209, 191, 55));
	rs.setOutlineThickness(1.8f);
	rs.setOrigin(20.f, 250.f);
	rs.setPosition(pos.x, 540);
	wnd->draw(rs);
}

void PowerOil::drawPowerIdle(RenderWindow* wnd)
{
	Vector2f p(Mouse::getPosition(*wnd));
	RectangleShape rs(Vector2f(40.f, 500.f));
	rs.setFillColor(Color(219, 201, 65, 150));
	rs.setOutlineColor(Color(219, 201, 65));
	rs.setOutlineThickness(2.f);
	rs.setOrigin(20.f, 250.f);
	rs.setPosition(p.x, 540);
	wnd->draw(rs);
}

//// FREEZE ////

void PowerFreeze::onPowerStop()
{
	Game::instance->setGameSpeed(Game::instance->getGameSpeed() * 3.f);
}

void PowerFreeze::drawPower(RenderWindow * wnd)
{
	int effectAlpha = (-std::pow(Game::instance->powerTime, 2.f) + 500.f*Game::instance->powerTime + 250.f) / (62750.f / 64.f);

	RectangleShape rs(Vector2f(1920.f, 500.f));
	rs.setPosition(0.f, 540.f);
	rs.setOrigin(0.f, 250.f);
	rs.setFillColor(Color(164, 241, 249, effectAlpha));
	wnd->draw(rs);
}

bool PowerFreeze::onPowerStart()
{
	Game::instance->setGameSpeed(Game::instance->getGameSpeed() / 3.f);
	return true;
}
