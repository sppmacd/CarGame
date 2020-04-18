#pragma once

#include "Power.h"

class PowerBall : public Power
{
public:
    struct PbBall
    {
        float pos;
        int lane;
        int destroyAnim;
        void update();
    };

    PowerBall();
	virtual bool onPowerStart();
	virtual void onPowerStop();
	virtual void onPowerTick(int powerTick);
	virtual void onCooldownTick(int tick);
	virtual void onTextureLoad();
	virtual void drawPower(RenderWindow* wnd);
	virtual void drawPowerIdle(RenderWindow* wnd);
	virtual int getCost();
    virtual string getName();

	Vector2f pos;
	int lane;
	std::vector<PbBall> balls;
};
