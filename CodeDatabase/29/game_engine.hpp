#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include "command_parser.hpp"
#include "renderer.hpp"
#include "game.hpp"
#include "input.hpp"

template <typename CommandSource, typename Visualizer>
class GameEngine {
public:
	GameEngine(Game& game, CommandSource source, Visualizer visualizer)
		: game_(game), source_(std::move(source)), visualizer_(std::move(visualizer)) {}

	void RunLoop() {
		while (true) {
			visualizer_.OnGameUpdated(game_);
			UserCommand cmd = source_.NextCommand();
			if (cmd.type == UserCommandType::Exit) break;
			if (cmd.type == UserCommandType::Save || cmd.type == UserCommandType::Load) {
				continue;
			}
			std::string commandString = ToLegacyCommand(cmd);
			if (!commandString.empty()) {
				Game* g = &game_;
				Input dummy(std::cin, std::cout);
				g->PlayerTurn(commandString, dummy);
			}
			if (game_.IsVictory() || game_.IsPlayerDead()) break;
			game_.EnemiesTurn();
			if (game_.IsVictory() || game_.IsPlayerDead()) break;
		}
	}

private:
	std::string ToLegacyCommand(const UserCommand& cmd) {
		switch (cmd.type) {
			case UserCommandType::MoveUp: return "W";
			case UserCommandType::MoveDown: return "S";
			case UserCommandType::MoveLeft: return "A";
			case UserCommandType::MoveRight: return "D";
			case UserCommandType::AttackUp: return "U";
			case UserCommandType::AttackDown: return "V";
			case UserCommandType::AttackLeft: return "L";
			case UserCommandType::AttackRight: return "R";
			case UserCommandType::ToggleMode: return "T";
			case UserCommandType::Cast: {
				if (cmd.slot > 0) {
					return std::to_string(cmd.slot) + " " + std::to_string(cmd.targetX) + " " + std::to_string(cmd.targetY);
				}
				return {};
			}
			default: return {};
		}
	}

	Game& game_;
	CommandSource source_;
	Visualizer visualizer_;
};

class CommandStreamSource {
public:
	CommandStreamSource(Input& input, const CommandParser& parser) : input_(input), parser_(parser) {}
	UserCommand NextCommand() {
		std::string line = input_.ReadLine("");
		return parser_.Parse(line);
	}
private:
	Input& input_;
	CommandParser parser_;
};

#endif
