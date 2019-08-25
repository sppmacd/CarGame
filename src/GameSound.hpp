#pragma once

#include <SFML/Audio.hpp>
#include <vector>
#include <map>
#include <string>

struct SoundBase
{
    std::vector<sf::SoundBuffer> buffers;
public:
    // Construct sound. Add more buffers if you want to use random sounds.
    SoundBase(std::vector<sf::SoundBuffer> buffs);

    // Starts playing this sound and returns SFML handle. If random, returns random-created sound.
    sf::Sound* playSound(float volume);
};

class GameSound
{
    std::map<std::string, SoundBase> soundBuffers;
    std::vector<std::pair<std::string, sf::Sound*>> playedSounds;
public:
    float soundVolume;

    GameSound();

    // Adds sound to map.
    bool loadSound(std::string name, bool random = false);

    // Reloads all sounds.
    void reload();

    // Starts playing sound. Plays random sound if the sound is random.
    void playSound(std::string name, float volume);

    // Stops playing all sounds with this name.
    void stopSounds(std::string name);

    // Pauses playing all sounds with this name.
    void pauseSounds(std::string name);

    // Sets volume of sounds with this name.
    void setVolume(std::string name, float volume);

    // Sets sound looped.
    void setSoundLooped(bool loop);

    // Update sounds - removes it from map if sound playing is finished.
    void update();
};
