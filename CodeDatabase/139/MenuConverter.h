#pragma once
#include "IConverter.h"


class Game;

class MenuConverter : public IConverter
{
	Game* game;
public:
	MenuConverter(Game* game);
	Command* convert(const InputEvent& event) override;
};