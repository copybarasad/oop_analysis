#pragma once
#include "IConverter.h"


class Game;

class GameWinConverter : public IConverter
{
	Game* game;
public:
	GameWinConverter(Game* game);
	Command* convert(const InputEvent& event) override;
};