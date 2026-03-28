#pragma once
#include "IConverter.h"


class Game;

class LevelBetweenConverter : public IConverter
{
	Game* game;
public:
	LevelBetweenConverter(Game* game);
	Command* convert(const InputEvent& event) override;
};