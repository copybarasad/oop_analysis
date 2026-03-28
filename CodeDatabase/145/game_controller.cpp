#include "game_controller.h"
#include "game.h"
#include "action_commands.h"
#include "movement_commands.h"
#include "console_input_reader.h"
#include <memory>

template<>
std::unique_ptr<Command> GameController<ConsoleInputReader>::convertGameCommandToCommand(const GameCommand& cmd) {
    switch (cmd.type) {
        case GameCommand::Type::MOVE_UP:
            return std::make_unique<MoveUpCommand>();
        case GameCommand::Type::MOVE_DOWN:
            return std::make_unique<MoveDownCommand>();
        case GameCommand::Type::MOVE_LEFT:
            return std::make_unique<MoveLeftCommand>();
        case GameCommand::Type::MOVE_RIGHT:
            return std::make_unique<MoveRightCommand>();
        case GameCommand::Type::SWITCH_MODE:
            return std::make_unique<SwitchModeCommand>();
        case GameCommand::Type::RANGED_ATTACK:
            return std::make_unique<RangedAttackCommand>();
        case GameCommand::Type::CAST_SPELL:
            return std::make_unique<CastSpellCommand>(cmd.spellIndex);
        case GameCommand::Type::SAVE_GAME:
            return std::make_unique<SaveGameCommand>(cmd.filename);
        case GameCommand::Type::LOAD_GAME:
            return std::make_unique<LoadGameCommand>(cmd.filename);
        case GameCommand::Type::QUIT:
            return std::make_unique<QuitCommand>();
        case GameCommand::Type::INVALID:
        default:
            return std::make_unique<InvalidCommand>();
    }
}

template class GameController<ConsoleInputReader>;
