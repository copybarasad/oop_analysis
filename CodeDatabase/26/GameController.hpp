#pragma once

#include <string>
#include <utility>

#include "EventBus.hpp"
#include "Game.hpp"
#include "GameCommand.hpp"

template <typename InputProvider>
class GameController {
public:
	GameController(Game& game,
	               InputProvider inputProvider,
	               EventBus& bus)
		: game_(game),
		  input_(std::move(inputProvider)),
		  bus_(bus) {}

	void run() {
		while (!game_.isExitRequested()) {
			GameCommand command = input_.readCommand();
			if (command.type() == GameCommand::Type::None) {
				continue;
			}
			bus_.publish(GameEvent::makeMessage(
			    GameEvent::Type::CommandIssued,
			    std::string("Command received: ") + commandName(command.type())));
			game_.handleCommand(command);
		}
	}

private:
	static const char* commandName(GameCommand::Type type) {
		switch (type) {
		case GameCommand::Type::MoveUp: return "MoveUp";
		case GameCommand::Type::MoveDown: return "MoveDown";
		case GameCommand::Type::MoveLeft: return "MoveLeft";
		case GameCommand::Type::MoveRight: return "MoveRight";
		case GameCommand::Type::AttackUp: return "AttackUp";
		case GameCommand::Type::AttackDown: return "AttackDown";
		case GameCommand::Type::AttackLeft: return "AttackLeft";
		case GameCommand::Type::AttackRight: return "AttackRight";
		case GameCommand::Type::SwitchMode: return "SwitchMode";
		case GameCommand::Type::CastSpell1: return "Spell1";
		case GameCommand::Type::CastSpell2: return "Spell2";
		case GameCommand::Type::CastSpell3: return "Spell3";
		case GameCommand::Type::CastSpell4: return "Spell4";
		case GameCommand::Type::CastSpell5: return "Spell5";
		case GameCommand::Type::SaveGame: return "SaveGame";
		case GameCommand::Type::LoadGame: return "LoadGame";
		case GameCommand::Type::QuitToMenu: return "QuitToMenu";
		case GameCommand::Type::StartCampaign: return "MenuStart";
		case GameCommand::Type::LoadCampaignFromMenu: return "MenuLoad";
		case GameCommand::Type::ExitApplication: return "Exit";
		default: return "None";
		}
	}

	Game& game_;
	InputProvider input_;
	EventBus& bus_;
};
