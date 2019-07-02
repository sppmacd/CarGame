#include "PowerFence.hpp"
#include "Car.h"
#include "GameDisplay.h"
#include "Game.h"
#include "Level.h"

//// FENCE ////

PowerFence::PowerFence() : Power()
{
    setMaxTime(600);
}
void PowerFence::onPowerStop()
{
    for (Car* c : Game::instance->cars)
    {
        if(c->getScreenPos().x < pos.x - 60.f)
        {
            c->makeDestroy(c->maxHealth);
        }
    }
}

bool PowerFence::onPowerStart()
{
	pos = Vector2f(Mouse::getPosition(*GameDisplay::instance->getRenderWnd()));
	lane = LevelUtility::getLaneFromPos(pos);
	if(lane < 0 || lane > 2)
        return false;
	return true;
}

void PowerFence::onPowerTick(int powerTick)
{
    for (Car* c : Game::instance->cars)
    {
        if(c->getScreenPos().x < pos.x + 110.f)
        {
            if(     (lane == 0 && c->getLine() < 2) ||
                    (lane == 1) ||
                    (lane == 2 && c->getLine() > 0) )
            {
                c->pos = (pos.x + 110.f) / 2.f;
                if((lane != 1 || powerTick % 2 == 0) && !c->isDestroying())
                    c->makeDestroy(0.2f);
                c->setSpeed(0.f);
            }
        }
    }
}

void PowerFence::drawPower(RenderWindow * wnd)
{
	RectangleShape rs(Vector2f(20.f, 180.f));
	rs.setFillColor(Color(102, 48, 19));
	rs.setOutlineColor(Color(82, 38, 14));
	rs.setOutlineThickness(1.8f);
	rs.setOrigin(10.f, 90.f);

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

void PowerFence::drawPowerIdle(RenderWindow* wnd)
{
	Vector2f p(GameDisplay::instance->mousePos());

	RectangleShape rs(Vector2f(20.f, 167.f));
	rs.setFillColor(Color(102, 48, 19, 180));
	rs.setOutlineColor(Color(82, 38, 14, 180));
	rs.setOutlineThickness(1.8f);
	rs.setOrigin(10.f, 83.5f);

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
int PowerFence::getCost()
{
    return 4000;
}
string PowerFence::getName()
{
    return "fence";
}
