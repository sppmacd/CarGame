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
    ballTexture = NULL;
}

void PowerBall::onTextureLoad()
{
    GameDisplay& gameDisplay = *GameDisplay::instance;
    gameDisplay.addTexture("misc/power/ball");
    ballTexture = &gameDisplay.getTexture("misc/power/ball");
}

void PowerBall::onPowerStop()
{
}

void PowerBall::PbBall::update()
{
    if(destroyAnim != -1)
    {
        pos -= 4.3f * (destroyAnim / 30.f);
        destroyAnim--;

        // find cars that will be pushed back and push them back
        for(Car* c : Game::instance->cars)
        {
            if(abs(c->getScreenPos().x - pos) < 132.f && c->getLine() == lane)
            {
                float fac = 0.5 * (Power::getCurrentPowerLevel() - 1) / 4.f;
                c->setSpeed(c->getSpeed() - fac);
            }
        }
    }
    else
    {
        pos += 4.3f;

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
    if((1200 - powerTick) % 90 == 0)
    {
        PowerBall::PbBall ball;
        ball.destroyAnim = -1;
        ball.lane = rand() % 3;
        ball.pos = -32.f;
        ball.lifeTime = 0;
        balls.push_back(ball);
    }
    //cout << "PowerBall: BALLS:" << balls.size() << endl;
    for(int i = 0; i < balls.size(); i++)
    {
        PowerBall::PbBall& ball = balls[i];
        ball.update();
        ball.lifeTime++;
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
    if(balls.size() > 0 && tick % 5 == 0)
    {
        int i = rand() % balls.size();
        if(balls[i].destroyAnim == -1)
            balls[i].destroyAnim = 30; //remove random ball every 5t
    }
}

void PowerBall::drawPower(RenderWindow * wnd)
{
    // TODO
    for(int i = 0; i < balls.size(); i++)
    {
        PowerBall::PbBall& ball = balls[i];
        sf::Vector2f vec = LevelUtility::getLanePos(ball.lane);

        // some simple draw
        /*
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

        wnd->draw(cs);*/

        sf::IntRect rect;

        static const int TEX_SIZE = 64;
        static const int FRAME_TICKS = 5;
        static const int DESTROY_FRAMES = 8;

        rect.width = TEX_SIZE;
        rect.height = TEX_SIZE;

        if(ball.destroyAnim == -1)
        {
            rect.left = 0;
            rect.top = (ball.lifeTime / FRAME_TICKS) % 4 * 64;
        }
        else
        {
            int frame = 8 - (ball.destroyAnim / 30.f) * DESTROY_FRAMES;
            rect.left = (frame / 2) * TEX_SIZE;
            rect.top = (frame % 4) * TEX_SIZE;
        }

        sf::Sprite sprite = sf::Sprite(*ballTexture, rect);
        sprite.setOrigin(32.f, 32.f);
        sprite.setPosition(ball.pos, vec.y);
        wnd->draw(sprite);
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
