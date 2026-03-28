#pragma once
#include "GameLogic.h"
#include "IInputReader.h"
struct GameContext {
	GameWorld& world;
	GameLogic& logic;
	EventSystem& events;
	IInputReader& input_reader;
};
