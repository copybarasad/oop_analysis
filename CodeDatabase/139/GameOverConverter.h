#pragma once
#include "IConverter.h"


class Game;

class GameOverConverter : public IConverter
{
	Game* game;
public:
	GameOverConverter(Game* game);
	Command* convert(const InputEvent& event) override;
};
