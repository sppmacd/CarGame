#include "PowerOil.hpp"
#include "Car.h"
#include "GameDisplay.h"
#include "Game.h"
#include "Level.h"

//// OIL ////

PowerOil::PowerOil() : Power() {}
void PowerOil::onPowerStop()
{
	Game::instance->setGameSpeed(Game::instance->getGameSpeed() / 1.3f);
}

bool PowerOil::onPowerStart()
{
	Game::instance->setGameSpeed(Game::instance->getGameSpeed() * 1.3f);
	pos = Vector2f(Mouse::getPosition(*GameDisplay::instance->getRenderWnd()));
	lane = LevelUtility::getLaneFromPos(pos);
	if(lane < 0 || lane > 2)
        return false;
	return true;
}

void PowerOil::onPowerTick(int powerTick)
{
	if (powerTick % 4 == 0)
	{
		for (Car* c : Game::instance->cars)
		{
			if(abs(c->getScreenPos().x - pos.x) < 45)
			{
			    if(lane == 0 && c->getLine() < 2)
                    c->makeDestroy();
                else if(lane == 1 && (c->getLine() == 1 || (powerTick % 8 == 0)))
                    c->makeDestroy();
                else if(lane == 2 && c->getLine() > 0)
                    c->makeDestroy();
			}
		}
	}
}

void PowerOil::drawPower(RenderWindow * wnd)
{
	RectangleShape rs(Vector2f(40.f, 180.f));
	rs.setFillColor(Color(219, 201, 65, 180));
	rs.setOutlineColor(Color(209, 191, 55));
	rs.setOutlineThickness(1.8f);
	rs.setOrigin(20.f, 90.f);

	switch(lane)
	{
    case 0:
        rs.setPosition(pos.x, (LevelUtility::getLanePos(0).y + LevelUtility::getLanePos(1).y) / 2.f);
        break;
    case 1:
        rs.setPosition(pos.x, LevelUtility::getLanePos(1).y);
        break;
    case 2:
        rs.setPosition(pos.x, (LevelUtility::getLanePos(1).y + LevelUtility::getLanePos(2).y) / 2.f);
        break;
    default:
        return;
	}
	wnd->draw(rs);
}

void PowerOil::drawPowerIdle(RenderWindow* wnd)
{
	Vector2f p(GameDisplay::instance->mousePos());

	RectangleShape rs(Vector2f(40.f, 167.f));
	rs.setFillColor(Color(219, 201, 65, 180));
	rs.setOutlineColor(Color(209, 191, 55));
	rs.setOutlineThickness(1.8f);
	rs.setOrigin(20.f, 83.5f);

	int ln = LevelUtility::getLaneFromPos(p); //lane
	switch(ln)
	{
    case 0:
        rs.setPosition(p.x, (LevelUtility::getLanePos(0).y + LevelUtility::getLanePos(1).y) / 2.f);
        break;
    case 1:
        rs.setPosition(p.x, LevelUtility::getLanePos(1).y);
        break;
    case 2:
        rs.setPosition(p.x, (LevelUtility::getLanePos(1).y + LevelUtility::getLanePos(2).y) / 2.f);
        break;
    default:
        return;
	}
	wnd->draw(rs);
}
