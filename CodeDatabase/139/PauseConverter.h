#pragma once
#include "IConverter.h"


class Game;


class PauseConverter : public IConverter
{
	Game* game;
public:
	PauseConverter(Game* game);
	Command* convert(const InputEvent& event) override;
};