#include "GameSound.hpp"
#include <cstdlib>
#include <iostream>
#include "GameDisplay.h"
#include "Game.h"
#include "DebugLogger.hpp"

SoundBase::SoundBase(std::vector<sf::SoundBuffer> buffs): buffers(buffs)
{

}

sf::Sound* SoundBase::playSound(float volume)
{
    sf::Sound* sound = new sf::Sound(buffers[rand() % buffers.size()]);
    sound->setVolume(volume);
    return sound;
}

////////////////////////////////////////////////////////////////

GameSound::GameSound()
{
}

bool GameSound::loadSound(std::string name, std::string mod, bool random)
{
    DebugLogger::logDbg("Adding sound: " + mod + ":" + name + "(random=" + std::to_string(random) + ")", "GameSound");
    std::vector<sf::SoundBuffer> buffers;
    if(!random)
    {
        DebugLogger::logDbg("Adding basic SoundBuffer: " + mod + ":" + name, "GameSound");
        sf::SoundBuffer buf;
        if(buf.loadFromFile("res/" + mod + "/sounds/" + name + ".ogg"))
        {
            buffers.push_back(buf);
        }
        else
        {
            DebugLogger::log("Sound not loaded: " + mod + ":" + name, "GameSound", "ERROR");
            return false;
        }
    }
    else
    {
        size_t c = 1;
        bool loaded = true;
        while(loaded)
        {
            DebugLogger::logDbg("Adding randomized SoundBuffer: " + mod + ":" + name + std::to_string(c), "GameSound");
            sf::SoundBuffer buf;
            loaded = buf.loadFromFile("res/" + mod + "/sounds/" + name + std::to_string(c) + ".ogg");
            if(!loaded && c == 1)
            {
                DebugLogger::log("Random sound not loaded: " + mod + ":" + name, "GameSound", "ERROR");
                return false;
            }
            if(loaded)
            {
                buffers.push_back(buf);
            }
            c++;
        }
    }
    SoundBase base(buffers);
    //std::cout << "Adding sound " << name << std::endl;
    soundBuffers.insert(make_pair(name, base));
    return true;
}

void GameSound::reload()
{
    GameDisplay::loadingStr = "Reloading resources: Sounds...";
    DebugLogger::log("Reloading sounds...", "GameSound");

    soundVolume = Game::instance->settings.getSettingNumeric("volume", "sound");
    soundBuffers.clear();
    bool err = false;

    err |= ! loadSound("click", "api");
    err |= ! loadSound("damage", "api", true);
    err |= ! loadSound("destroy", "api");
    err |= ! loadSound("start", "api");
    err |= ! loadSound("coin_add", "api");
    err |= ! loadSound("coin_remove", "api");
    err |= ! loadSound("point_add", "api");
    err |= ! loadSound("coin_mpl", "api");
    err |= ! loadSound("game_over", "api");
    err |= ! loadSound("upgrade", "api");
}

void GameSound::playSound(std::string name, float volume)
{
    auto it = soundBuffers.find(name);
    if(it != soundBuffers.end())
    {
        sf::Sound* sound = it->second.playSound(volume * soundVolume);
        //std::cout << "n=" << name << "v=" << volume * soundVolume << std::endl;
        sound->play();
        playedSounds.push_back(make_pair(name, sound));
    }
    else
    {
        DebugLogger::log("Tried to play invalid sound: " + name, "GameSound", "WARN");
    }
}

void GameSound::stopSounds(std::string name)
{
    for(auto it = playedSounds.begin(); it != playedSounds.end(); it++)
    {
        if(name.empty() || name == it->first)
            it->second->stop();
    }
}

void GameSound::pauseSounds(std::string name)
{
    for(auto it = playedSounds.begin(); it != playedSounds.end(); it++)
    {
        if(name.empty() || name == it->first)
            it->second->pause();
    }
}

void GameSound::setVolume(std::string name, float volume)
{
    for(auto it = playedSounds.begin(); it != playedSounds.end(); it++)
    {
        if(name == it->first || name.empty())
            it->second->setVolume(volume);
    }
}

void GameSound::update()
{
    for(size_t s = 0; s < playedSounds.size(); s++)
    {
        // Erase stopped sounds.
        if(playedSounds[s].second->getStatus() == sf::SoundSource::Stopped)
        {
            DebugLogger::logDbg("Removing sound: " + playedSounds[s].first, "GameSound");
            delete playedSounds[s].second;
            playedSounds.erase(playedSounds.begin() + s);
        }
    }
}
