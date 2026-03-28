#pragma once
#include "IConverter.h"
#include <string>

class KeyConfiguration;
class Game;

class PlayConverter : public IConverter
{
private:
	Game* game;
	KeyConfiguration* config;
public:
	PlayConverter(Game* game);
	Command* convert(const InputEvent& event) override;
	~PlayConverter();
};