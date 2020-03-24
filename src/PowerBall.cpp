#include "PowerBall.hpp"
#include "Car.h"
#include "GameDisplay.h"
#include "Game.h"
#include "Level.h"
#include <cmath>
#include <iostream>

PowerBall::PowerBall() : Power()
{
    setMaxTime(1200);
}
void PowerBall::onPowerStop()
{
	for(int i = 0; i < balls.size(); i++)
    {
        balls[i].destroyAnim = 30;
    }
}

void PowerBall::PbBall::update()
{
    if(destroyAnim != -1)
    {
        pos -= 4.5f * (destroyAnim / 30.f);
        destroyAnim--;

        // push back car
        // TODO

        // find cars that will be pushed back
        for(Car* c : Game::instance->cars)
        {
            if(abs(c->getScreenPos().x - pos) < 132.f && c->getLine() == lane)
            {
                float fac = 1.8 * (Power::getCurrentPowerLevel() - 1) / 4.f;
                c->setSpeed(c->getSpeed() - (destroyAnim > 15 ? fac : -fac));
                pos -= c->getSpeed();
            }
        }
    }
    else
    {
        pos += 4.5f;

        // check if will damage some cars
        for(Car* c : Game::instance->cars)
        {
            if(abs(c->getScreenPos().x - pos) < 132.f && c->getLine() == lane && !c->isDestroying())
            {
                c->makeDestroy(0.4f * Power::getCurrentPowerLevel());
                destroyAnim = 30;
            }
        }
    }

    // destroy ball if gets out of screen
    if(pos > GameDisplay::instance->getSize().x + 32)
    {
        destroyAnim = 0;
    }
}

bool PowerBall::onPowerStart()
{
    balls.clear();
	return true;
}

void PowerBall::onPowerTick(int powerTick)
{
    // TODO
    if((1200 - powerTick) % 30 == 0)
    {
        PowerBall::PbBall ball;
        ball.destroyAnim = -1;
        ball.lane = rand() % 3;
        ball.pos = -32.f;
        balls.push_back(ball);
    }
    //cout << "PowerBall: BALLS:" << balls.size() << endl;
    for(int i = 0; i < balls.size(); i++)
    {
        PowerBall::PbBall& ball = balls[i];
        ball.update();
        //cout << "PowerBall: BALL " << i << " POS:" << ball.pos << "," << ball.lane << endl;
        if(ball.destroyAnim == 0)
        {
            balls.erase(balls.begin() + i);
            i--;
            continue;
        }
    }
}

void PowerBall::onCooldownTick(int tick)
{
    onPowerTick(1); //DO NOT SPAWN BALLS DURING COOLDOWN
}

void PowerBall::drawPower(RenderWindow * wnd)
{
    // TODO
    for(int i = 0; i < balls.size(); i++)
    {
        PowerBall::PbBall& ball = balls[i];
        sf::Vector2f vec = LevelUtility::getLanePos(ball.lane);

        // some simple draw
        sf::CircleShape cs(32.f);
        cs.setOrigin(32.f, 32.f);
        cs.setPosition(ball.pos, vec.y);
        if(ball.destroyAnim != -1)
        {
            cs.setFillColor(Color::Red);
        }
        else
        {
            cs.setFillColor(Color::Green);
        }

        wnd->draw(cs);
    }
}

void PowerBall::drawPowerIdle(RenderWindow* wnd)
{
    drawPower(wnd);
}
int PowerBall::getCost()
{
    return 8000;
}
string PowerBall::getName()
{
    return "ball";
}
