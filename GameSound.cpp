#include "GameSound.h"
#include "GameDisplay.h"

GameSound* GameSound::instance;

GameSound::GameSound()
{
    instance = this;

    GameDisplay::drawLoadingProgress("Loading Sound Engine...", GameDisplay::instance->getRenderWnd());

    this->addSound("click"); //0
    this->addSound("destroy"); //1
}

void GameSound::addSound(string name)
{
    sf::SoundBuffer buffer;

    if(!buffer.loadFromFile("res/" + name + ".ogg"))
    {
        cout << "GameSound: Cannot load sound " << name << endl;
        return;
    }
    this->soundBuffers.push_back(buffer);

    cout << "GameSound: Loaded sound " << name << endl;
}

sf::SoundBuffer GameSound::getBuffer(int id)
{
    return this->soundBuffers[id];
}
