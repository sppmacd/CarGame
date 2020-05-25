#include "PowerOil.hpp"
#include "CoreLoader.hpp"
#include <cmath>

//// OIL ////

PowerOil::PowerOil()
    : Power("cgcore") {}
void PowerOil::onPowerStop()
{
	//Game::instance->setGameSpeed(Game::instance->getGameSpeed() / (1.3f / ((-1 / currentLevel) + 1)));
}

bool PowerOil::onPowerStart()
{
	//Game::instance->setGameSpeed(Game::instance->getGameSpeed() * (1.3f / ((-1 / currentLevel) + 1)));
	pos = (Vector2f)GameDisplay::instance->mousePos();
	lane = LevelUtility::getLaneFromPos(pos);
	if(lane < 0 || lane > 2)
        return false;
	return true;
}

void PowerOil::onPowerTick(int powerTick)
{
    int size = currentLevel * 45;
	if(powerTick % 3 == 0)
	{
		for(Car* c : Game::instance->cars)
		{
			if(abs(c->getScreenPos().x - pos.x) < size)
			{
			    if(lane == 0 && c->getLine() < 2)
                    c->makeDestroy(0.2 * currentLevel + 0.3);
                else if(lane == 1 && (c->getLine() == 1 || (powerTick % 6 == 0)))
                    c->makeDestroy(0.2 * currentLevel + 0.3);
                else if(lane == 2 && c->getLine() > 0)
                    c->makeDestroy(0.2 * currentLevel + 0.3);
			}
		}
	}
}

void PowerOil::drawPower(RenderWindow * wnd)
{
	RectangleShape rs(Vector2f(40.f, 180.f));
	rs.setFillColor(Color(219, 201 / (currentLevel / 2.f), 65, 180));
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
int PowerOil::getCost()
{
    return 400;
}
string PowerOil::getName()
{
    return "oil";
}
