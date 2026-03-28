#pragma once
#include "Commands/Command.h"
#include "Commands/Converters/IConverter.h"
#include <optional>
#include <utility>

class Game;

template <typename Reader>
class GameController
{
	Reader reader;
	Game* game;
public:
	GameController(Game* game, Reader&& reader) 
		: game(game), reader(std::move(reader)) { }

	void handle()
	{
		while (auto event = reader.getInputEvent()) {
			if (event.has_value())
			{
				Command* cmd = game->getCurrentState()->getConverter()->convert(event.value());
				if (cmd != nullptr) {
					cmd->execute();
					delete cmd;
				}
			}
			else
			{
				break;
			}
		}
	}
};