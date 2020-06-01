#include "GameDisplay.h"
#include "Game.h"

#include <cstdlib>
#include <cmath>

void GameDisplay::drawStat(int x, int y, String texture, long val, int animTick)
{
    Texture& _texture = this->getTexture("api$stat/" + texture);
    sf::Sprite s1(_texture);
    s1.setPosition(x, y);
    s1.setScale(64.f / _texture.getSize().x, 64.f / _texture.getSize().y);
    s1.setOrigin(_texture.getSize().x /2.f, _texture.getSize().y /2.f);
    this->renderWnd->draw(s1);

    sf::Text t1 = this->drawString(to_string(val), 45, sf::Vector2f(x + 48.f, y), sf::Text::Bold);
    t1.setFillColor(sf::Color::Cyan);
    t1.setOutlineColor(sf::Color::Black);
    t1.setOutlineThickness(1.f);
    t1.setOrigin(t1.getLocalBounds().width / 2.f, t1.getLocalBounds().height / 2.f);
    t1.move(t1.getLocalBounds().width / 2.f, -t1.getLocalBounds().height / 2.f);
    t1.setScale(animTick / 30.f + 1.f, animTick / 30.f + 1.f);
    this->renderWnd->draw(t1);

    if(sf::IntRect(x - 32, y - 32, 64, 64).contains(Game::instance->paused() ? sf::Mouse::getPosition(*renderWnd) : mousePos()) || Game::instance->playerData.isNewPlayer)
    {
        t1.move(0.f, 30.f);
        t1.setOrigin(0.f, 0.f);
        t1.setFillColor(Color::Cyan);
        t1.setCharacterSize(20);
        t1.setScale(1.f, 1.f);
        t1.setString(Game::instance->translation.get("stat."+texture));

        renderWnd->draw(t1);
    }
}

void GameDisplay::drawLoading(sf::RenderWindow* wnd)
{
	static int animC = 0;
	wnd->clear(Color(30, 30, 40));
	if(GameDisplay::consoleStr.isEmpty())
    {
        GameDisplay::drawLoadingProgress(loadingStr, wnd);
        animC++;

        int aX = animC % 400;
        sf::RectangleShape rsbg(Vector2f(100.f, 20.f));
        rsbg.setFillColor(Color(20, 20, 20));
        bool err = GameDisplay::instance && GameDisplay::instance->error;
        rsbg.setOutlineColor(err ? Color::Red : Color(70, 70, 70));
        rsbg.setOutlineThickness(2.f);
        rsbg.setOrigin(50.f, 10.f);
        rsbg.setPosition(wnd->getSize().x / 2, wnd->getSize().y / 2 + 200);
        wnd->draw(rsbg);

        sf::RectangleShape rs(Vector2f(3.f, 20.f));
        rs.setPosition(wnd->getSize().x / 2 + aX*0.25f - 50, wnd->getSize().y / 2 + 200);
        rs.setOrigin(1.5f, 10.f);
        rs.setFillColor(Color(230, 230, 230));
        rs.setOutlineColor(Color(50,50,50));
        rs.setOutlineThickness(1.5f);
        wnd->draw(rs);
    }
    else
    {
        // Quickly load the font !!
        static bool error = false;
        static Font guiFont;
        if(guiFont.getInfo().family.empty() && !error && !guiFont.loadFromFile("res/font.ttf"))
        {
            error = true;
            return;
        }
        // Draw text with default settings.
        sf::Text text(consoleStr, guiFont, 15);
        wnd->draw(text);
    }

	wnd->display();
}

void GameDisplay::drawTutorial(sf::Vector2f cp, sf::Vector2f cs, String str)
{
    sf::RectangleShape rs(cs);
    rs.setPosition(cp);
    rs.setFillColor(sf::Color(0, 0, 0, 0));
    rs.setOutlineColor(sf::Color(255, 255, 255));
    rs.setOutlineThickness(10.f);
    this->renderWnd->draw(rs);

    sf::Text tx = this->drawString(str, 30, sf::Vector2f(cp.x + cs.x + 10, cp.y), sf::Text::Bold);
    tx.setOutlineColor(sf::Color::Black);
    tx.setOutlineThickness(2.f);
    this->renderWnd->draw(tx);
}

void GameDisplay::drawSplash(String text)
{
	int textSize = (this->splashTick < 30 ? this->splashTick : 60 - this->splashTick)*2 + 30;
	sf::Text tx = drawCenteredString(text, textSize, Vector2f(getSize() / 2u), sf::Text::Bold);
	tx.setFillColor(Color(200, 20, 20));
	this->renderWnd->draw(tx);
}

void GameDisplay::setSplash(String text)
{
	this->splashText = text;
	this->splashSet = true;
	this->splashTick = 60;
}

Vector2i GameDisplay::mousePos()
{
	return Vector2i(this->getRenderWnd()->mapPixelToCoords(Mouse::getPosition(*this->getRenderWnd()), Game::instance->getGameView()));
}

void GameDisplay::drawDebugPie(sf::RenderWindow* wnd)
{
    GameDisplay* disp = GameDisplay::instance;
    Game* game = Game::instance;

    float dispY = disp->getSize().y;
    float dispX = disp->getSize().x;

    int t1 = game->times.timeEvent.asMicroseconds() - game->times.timeGui.asMicroseconds(); //non-CGUI events
    int t2 = game->times.timeGui.asMicroseconds(); //CGUI events
    int t3 = game->times.timeTick.asMicroseconds(); //Game logic
    int t4 = game->times.timeRender.asMicroseconds(); //Render
    int t5 = std::max(0LL, game->times.timeWait.asMicroseconds()); //FPS limit / Tick limit wait or lag time
    int tAll = t1+t2+t3+t4+t5;
    float ty1 = t1 * dispY / (tAll * 2.f);
    float ty2 = t2 * dispY / (tAll * 2.f);
    float ty3 = t3 * dispY / (tAll * 2.f);
    float ty4 = t4 * dispY / (tAll * 2.f);
    float ty5 = t5 * dispY / (tAll * 2.f);

    sf::RectangleShape r1(sf::Vector2f(40.f, ty1));
    sf::RectangleShape r2(sf::Vector2f(40.f, ty2));
    sf::RectangleShape r3(sf::Vector2f(40.f, ty3));
    sf::RectangleShape r4(sf::Vector2f(40.f, ty4));
    sf::RectangleShape r5(sf::Vector2f(40.f, ty5));

    r1.setPosition(dispX - 40, dispY - r1.getSize().y);
    r2.setPosition(dispX - 40, r1.getPosition().y - r2.getSize().y);
    r3.setPosition(dispX - 40, r2.getPosition().y - r3.getSize().y);
    r4.setPosition(dispX - 40, r3.getPosition().y - r4.getSize().y);
    r5.setPosition(dispX - 40, r4.getPosition().y - r5.getSize().y);

    r1.setFillColor(sf::Color::Red);
    r2.setFillColor(sf::Color::Green);
    r3.setFillColor(sf::Color::Blue);
    r4.setFillColor(sf::Color::Yellow);
    r5.setFillColor(sf::Color::Cyan);

    wnd->draw(r1);
    wnd->draw(r2);
    wnd->draw(r3);
    wnd->draw(r4);
    wnd->draw(r5);

    sf::Text tx1 = disp->drawString(String("P: " + to_string(t1) + " us"), 15, sf::Vector2f(disp->getSize().x - 200, disp->getSize().y - 21*1));
    sf::Text tx2 = disp->drawString(String("E: " + to_string(t2) + " us"), 15, sf::Vector2f(disp->getSize().x - 200, disp->getSize().y - 21*2));
    sf::Text tx3 = disp->drawString(String("T: " + to_string(t3) + " us"), 15, sf::Vector2f(disp->getSize().x - 200, disp->getSize().y - 21*3));
    sf::Text tx4 = disp->drawString(String("R: " + to_string(t4) + " us"), 15, sf::Vector2f(disp->getSize().x - 200, disp->getSize().y - 21*4));
    sf::Text tx5 = disp->drawString(String("W: " + to_string(t5) + " us"), 15, sf::Vector2f(disp->getSize().x - 200, disp->getSize().y - 21*5));

    tx1.setFillColor(sf::Color::White);
    tx1.setOutlineColor(sf::Color::Red);
    tx1.setOutlineThickness(1.f);
    tx2.setFillColor(sf::Color::White);
    tx2.setOutlineColor(sf::Color::Green);
    tx2.setOutlineThickness(1.f);
    tx3.setFillColor(sf::Color::White);
    tx3.setOutlineColor(sf::Color::Blue);
    tx3.setOutlineThickness(1.f);
    tx4.setFillColor(sf::Color::White);
    tx4.setOutlineColor(sf::Color::Yellow);
    tx4.setOutlineThickness(1.f);
    tx5.setFillColor(sf::Color::White);
    tx5.setOutlineColor(sf::Color::Cyan);
    tx5.setOutlineThickness(1.f);

    wnd->draw(tx1);
    wnd->draw(tx2);
    wnd->draw(tx3);
    wnd->draw(tx4);
    wnd->draw(tx5);
}

void GameDisplay::drawDebugInfo(sf::RenderWindow* wnd)
{
    GameDisplay* disp = GameDisplay::instance;

    sf::RectangleShape rs(sf::Vector2f(disp->getSize().x, 20));
    rs.setPosition(0.f, disp->getSize().y - 20);
    rs.setFillColor(sf::Color::Black);
    wnd->draw(rs);

	wnd->draw(disp->drawString(std::string("CG " + string(CG_VERSION))
		+ std::string(" |\ttick = ") + std::string(to_string(Game::instance->tickTime.asMicroseconds()))
		+ std::string(",\trtick = ") + std::string(to_string(Game::instance->realTickTime.asMicroseconds()))
		+ std::string(",\tfps = ") + std::string(to_string(int(1000000.f / Game::instance->realTickTime.asMicroseconds())))
		+ std::string(",\ttc = ") + std::string(to_string(Game::instance->tickCount))
		+ std::string(",\tmtc = ") + std::string(to_string(Game::instance->mainTickCount))
		+ std::string(",\tcars = ") + std::string(to_string(Game::instance->cars.size()))
		+ std::string(",\tg = ") + std::string(GameDisplay::instance->getVSync() ? "vsync" : "") + " " + std::string(Game::instance->isFullscreen() ? "fs" : "")
		+ std::string(",\tunpause = ") + std::string(to_string(Game::instance->unpauseDelay.asMicroseconds()))
		+ std::string(",\tdmg = ") + std::string(to_string(Game::instance->getDamageMultiplier()))
		+ std::string(",\tpt = ") + std::string(to_string(Game::instance->powerTime))
		+ std::string(",\tpc = ") + std::string(to_string(Game::instance->powerCooldown))
    , 15, sf::Vector2f(6.f, disp->getSize().y - 21)));

    drawDebugPie(wnd);
}

void GameDisplay::drawGui()
{
    Game* game = Game::instance;

    if(game->tickCount > 0/* || game->tutorialStep == 6*/)
    {
        bool bPowerUsable = game->usablePowerIds.size() != 0 && game->getCurrentPower() != "api$no_power";
        bool b2 = game->powerTime != 0;
        ModuleIdentifier currentPower = game->getCurrentPower();
        if(bPowerUsable || b2)
        {
            // Draw power cooldown / time
            sf::VertexArray arr(TriangleFan);
            Vector2f ccPos = Vector2f(1200,58);
            arr.append(Vertex(ccPos, Color(127,127,0)));
            double degtorad = 57.2957795;
            for(int i = 0; i <= 65; i++)
            {
                float deg = 360 * (i - 1) / 64.f;
                sf::Vector2f pointPos(32*sin(deg/degtorad), 32*cos(deg/degtorad));
                pointPos += ccPos;

                if(game->powerTime == -1)
                {
                    // todo
                    if(i >= game->powerCooldown / (game->maxPowerCooldown / 64.f))
                        arr.append(Vertex(pointPos, Color::Green));
                    else
                        arr.append(Vertex(pointPos, Color::Red));
                }
                else if(game->powerTime > 0)
                {
                    if(i >= game->powerTime / (game->maxPowerTime / 64.f))
                        arr.append(Vertex(pointPos, Color::Red));
                    else
                        arr.append(Vertex(pointPos, Color::Blue));
                }
                else
                {
                    arr.append(Vertex(pointPos, Color::Blue));
                }
            }
            this->renderWnd->draw(arr);

            ModuleIdentifier currentPwr = b2 ? ModuleIdentifier(game->powerHandle->id) : currentPower;

            sf::Sprite spr(this->getTexture(currentPwr.getModule() + "$power/" + currentPwr.getObjectId()));
            spr.setPosition(1050, 42);
            this->renderWnd->draw(spr);
        }
        if(bPowerUsable)
        {
            sf::Text t1 = this->drawCenteredString(game->translation.get("gui.powers.powerlvl", {to_string(game->playerData.powerLevels[game->getCurrentPower()]->getLevel())}), 45, sf::Vector2f(1050, 80), sf::Text::Bold);
            t1.setFillColor(sf::Color::Blue);
            this->renderWnd->draw(t1);
        }

        drawStat(260, 48, "score", game->getScore(), pointAnimTick);
        if(pointAnimTick > 0)
            pointAnimTick--;

        drawStat(470, 48, "high", game->playerData.highScore);
        drawStat(680, 48, "mpl", game->playerData.coinMpl);
        drawStat(890, 48, "points_mpl", game->playerData.pointsToNewMpl);
    }

    drawStat(50, 48, "coin", game->playerData.playerCoins);
	game->drawGui();

    if(game->debug) drawDebugInfo(this->renderWnd);
}

/*void GameDisplay::nextFullscreenMode()
{
	this->renderWnd->create(sf::VideoMode::getFullscreenModes()[fullscreenMode++], "CG " + string(CG_VERSION), sf::Style::Fullscreen);
	if(fullscreenMode == int(sf::VideoMode::getFullscreenModes().size()))
	{
		fullscreenMode = 0;
	}
	this->setVSync(this->getVSync());
	wndSizeDefault = this->renderWnd->getSize();
}*/
