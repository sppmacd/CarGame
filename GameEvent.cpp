#include "GameEvent.h"

GameEvent::GameEvent(const GameEvent & e)
{
	switch (type)
	{
	case GameEvent::CarSpawning:
		this->carSpawned.carToCreate = e.carSpawned.carToCreate;
		this->carSpawned.type = e.carSpawned.type;
	}
}
