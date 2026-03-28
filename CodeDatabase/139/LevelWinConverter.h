#pragma once
#include "IConverter.h"


class Game;

class LevelWinConverter : public IConverter
{
	Game* game;
public:
	LevelWinConverter(Game* game);
	Command* convert(const InputEvent& event) override;
};