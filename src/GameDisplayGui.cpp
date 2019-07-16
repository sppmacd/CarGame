#include "GameDisplay.h"
#include "Game.h"

#include <cstdlib>
#include <cmath>

void GameDisplay::drawStat(int x, int y, String texture, long val)
{
    sf::Sprite s1(this->getTexture("stat/" + texture));
    s1.setPosition(x, y + 10);
    this->renderWnd->draw(s1);

    sf::Text t1 = this->drawString(to_string(val), 45, sf::Vector2f(x + 50, y), sf::Text::Bold);
    t1.setFillColor(sf::Color::Blue);
    this->renderWnd->draw(t1);
}

void GameDisplay::drawLoading(sf::RenderWindow* wnd)
{
	static int animC = 0;
	wnd->clear(Color(30, 30, 40));
	GameDisplay::drawLoadingProgress(loadingStr, wnd);
	animC++;

	int aX = animC % 400;
	sf::RectangleShape rsbg(Vector2f(100.f, 20.f));
	rsbg.setFillColor(Color(20, 20, 20));
	rsbg.setOutlineColor(Color(70, 70, 70));
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

	wnd->display();
}

void GameDisplay::drawTutorial(sf::Vector2f cs, sf::Vector2f cp, String str)
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
	return Vector2i(this->getRenderWnd()->mapPixelToCoords(Mouse::getPosition(*this->getRenderWnd())));
}

void drawDebugPie(sf::RenderWindow* wnd)
{
    GameDisplay* disp = GameDisplay::instance;
    Game* game = Game::instance;

    sf::RectangleShape r1(sf::Vector2f(40.f, game->times.timeEvent.asMicroseconds() / 8.f));
    sf::RectangleShape r2(sf::Vector2f(40.f, game->times.timeGui.asMicroseconds() / 8.f));
    sf::RectangleShape r3(sf::Vector2f(40.f, game->times.timeTick.asMicroseconds() / 8.f));
    sf::RectangleShape r4(sf::Vector2f(40.f, game->times.timeRender.asMicroseconds() / 8.f));
    sf::RectangleShape r5(sf::Vector2f(40.f, game->times.timeWait.asMicroseconds() / 8.f));

    r1.setPosition(disp->getSize().x - 40, disp->getSize().y - r1.getSize().y);
    r2.setPosition(disp->getSize().x - 80, disp->getSize().y - r2.getSize().y);
    r3.setPosition(disp->getSize().x - 120, disp->getSize().y - r3.getSize().y);
    r4.setPosition(disp->getSize().x - 160, disp->getSize().y - r4.getSize().y);
    r5.setPosition(disp->getSize().x - 200, disp->getSize().y - r5.getSize().y);

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

    sf::Text tx1 = disp->drawString(String(to_string(game->times.timeEvent.asMicroseconds())), 15, sf::Vector2f(disp->getSize().x - 40, disp->getSize().y - 21));
    sf::Text tx2 = disp->drawString(String(to_string(game->times.timeGui.asMicroseconds())), 15, sf::Vector2f(disp->getSize().x - 80, disp->getSize().y - 21));
    sf::Text tx3 = disp->drawString(String(to_string(game->times.timeTick.asMicroseconds())), 15, sf::Vector2f(disp->getSize().x - 120, disp->getSize().y - 21));
    sf::Text tx4 = disp->drawString(String(to_string(game->times.timeRender.asMicroseconds())), 15, sf::Vector2f(disp->getSize().x - 160, disp->getSize().y - 21));
    sf::Text tx5 = disp->drawString(String(to_string(game->times.timeWait.asMicroseconds())), 15, sf::Vector2f(disp->getSize().x - 200, disp->getSize().y - 21));

    tx1.setFillColor(sf::Color::White);
    tx1.setOutlineColor(sf::Color::Black);
    tx1.setOutlineThickness(1.f);
    tx2.setFillColor(sf::Color::White);
    tx2.setOutlineColor(sf::Color::Black);
    tx2.setOutlineThickness(1.f);
    tx3.setFillColor(sf::Color::White);
    tx3.setOutlineColor(sf::Color::Black);
    tx3.setOutlineThickness(1.f);
    tx4.setFillColor(sf::Color::White);
    tx4.setOutlineColor(sf::Color::Black);
    tx4.setOutlineThickness(1.f);
    tx5.setFillColor(sf::Color::White);
    tx5.setOutlineColor(sf::Color::Black);
    tx5.setOutlineThickness(1.f);

    wnd->draw(tx1);
    wnd->draw(tx2);
    wnd->draw(tx3);
    wnd->draw(tx4);
    wnd->draw(tx5);
}

void drawDebugInfo(sf::RenderWindow* wnd)
{
    GameDisplay* disp = GameDisplay::instance;

    sf::RectangleShape rs(sf::Vector2f(disp->getSize().x, 20));
    rs.setPosition(0.f, disp->getSize().y - 20);
    rs.setFillColor(sf::Color::Black);
    wnd->draw(rs);

	wnd->draw(disp->drawString(std::string("CG " + string(CG_VERSION))
		+ std::string("|\ttick = ") + std::string(to_string(Game::instance->tickTime.asMicroseconds()))
		+ std::string(",\trtick = ") + std::string(to_string(Game::instance->realTickTime.asMicroseconds()))
		+ std::string(",\ttc = ") + std::string(to_string(Game::instance->tickCount))
		+ std::string(",\tmtc = ") + std::string(to_string(Game::instance->mainTickCount))
		+ std::string(",\tcars = ") + std::string(to_string(Game::instance->cars.size()))
		+ std::string(",\tg = ") + std::string(GameDisplay::instance->getVSync() ? "vsync" : "")
    , 15, sf::Vector2f(6.f, disp->getSize().y - 21)));

    drawDebugPie(wnd);
}

void GameDisplay::drawGui()
{
    Game* game = Game::instance;

    if(!game->paused()/* || game->tutorialStep == 6*/)
    {
        drawStat(250, 32, "score", game->getScore());
        drawStat(450, 32, "high", game->highScore);
        drawStat(650, 32, "mpl", game->getCoinMultiplier());
        drawStat(850, 32, "points_mpl", game->pointsToNewMpl);

        if(game->getCurrentPower() >= 0)
        {
            sf::Sprite spr(this->getTexture("power/" + to_string(game->getCurrentPower())));
            spr.setPosition(1050, 42);
            this->renderWnd->draw(spr);

            sf::Text t1 = this->drawCenteredString(game->translation.get("gui.powers.powerlvl", {to_string(game->powers[game->getCurrentPower()].getLevel())}), 45, sf::Vector2f(1050, 80), sf::Text::Bold);
            t1.setFillColor(sf::Color::Blue);
            this->renderWnd->draw(t1);
        }

        // Draw power cooldown
        sf::VertexArray arr(TriangleFan);
        Vector2f ccPos = Vector2f(1200,58);
        arr.append(Vertex(ccPos, Color(127,127,0)));
        double degtorad = 57.2957795;
        for(int i = 0; i <= 65; i++)
        {
            float deg = 360 * i / 64.f;
            sf::Vector2f pointPos(32*sin(deg/degtorad), 32*cos(deg/degtorad));
            pointPos += ccPos;

            if(game->powerTime == -1)
            {
                if(i >= game->powerCooldown / (1800.f / 64.f))
                    arr.append(Vertex(pointPos, Color::Green));
                else
                    arr.append(Vertex(pointPos, Color::Red));
            }
            else if(game->powerTime > 0)
            {
                if(i >= game->powerTime / (game->powerHandle->maxPowerTime / 64.f))
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
    }

	if(game->isGuiLoaded())
        game->drawGui(false);

    drawStat(50, 32, "coin", game->getCoins());

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
