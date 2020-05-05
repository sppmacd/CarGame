#ifndef API_HPP
#define API_HPP

// CarGame API headers
#include <cargame/AbilityBase.hpp>
#include <cargame/Car.h>
#include <cargame/DebugLogger.hpp>
#include <cargame/Game.h>
#include <cargame/GameDisplay.h>
#include <cargame/GameEvent.h>
#include <cargame/GameLoader.hpp>
#include <cargame/GameplayObjectManager.hpp>
#include <cargame/GameSound.hpp>
#include <cargame/LevelData.h>
#include <cargame/Level.h>
#include <cargame/Math.hpp>
#include <cargame/PlayerAbilityManager.hpp>
#include <cargame/PlayerDataManager.hpp>
#include <cargame/Power.h>
#include <cargame/SettingsManager.hpp>
#include <cargame/TranslationManager.hpp>

// CGUI headers
#include <CG/CG.h>

// SFML headers
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

// hmUtil headers
#include <HackerMan/Util/Main.hpp>

// DLL import/export
#define CGAPI SFML_SYSTEM_API

#endif // API_HPP

