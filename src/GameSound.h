#ifndef GameSoundH
#define GameSoundH

#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <iostream>

using namespace std;

class GameSound
{
public:
    static GameSound* instance;

    GameSound();
    sf::SoundBuffer getBuffer(int id);
    void addSound(string name);

    vector<sf::SoundBuffer> soundBuffers;
};

#endif // GameSoundH

