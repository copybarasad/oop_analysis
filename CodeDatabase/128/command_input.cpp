#include "command_input.hpp"
#include "icommand.hpp"
#include "usespellcommand.hpp"
#include "togglemode.hpp"
#include "savegamecommand.hpp"
#include "quitcommand.hpp"
#include "movecommand.hpp"
#include "loadgamecommand.hpp"
#include "enums.hpp"

#include <iostream>

CommandInput::CommandInput(): config_("config/keymap.cfg") {}

std::unique_ptr<ICommand> CommandInput::ReadCommand()
{
    std::cout << "Enter command: ";
    char c;
    if (!(std::cin >> c)) {
        return nullptr;
    }
    CommandType type = config_.Map(c);
    switch (type)
    {
        case CommandType::Quit:
            return std::make_unique<QuitCommand>();
        case CommandType::MoveUp:
            return std::make_unique<MoveCommand>(0, -1);
        case CommandType::MoveDown:
            return std::make_unique<MoveCommand>(0, 1);
        case CommandType::MoveLeft:
            return std::make_unique<MoveCommand>(-1, 0);
        case CommandType::MoveRight:
            return std::make_unique<MoveCommand>(1, 0);
        case CommandType::ToggleMode:
            return std::make_unique<ToggleModeCommand>();
        case CommandType::Save: {
            std::string filename;
            std::cout << "Save file: ";
            std::cin >> filename;
            return std::make_unique<SaveGameCommand>(filename);
        }
        case CommandType::Load: {
            std::string filename;
            std::cout << "Load file: ";
            std::cin >> filename;
            return std::make_unique<LoadGameCommand>(filename);
        }
        case CommandType::UseSpell: {
            return std::make_unique<UseSpellCommand>();
        }
        default:
            std::cout << "Unknown command.\n";
            return nullptr;
    }
}
