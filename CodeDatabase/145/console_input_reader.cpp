#include "console_input_reader.h"
#include "exceptions.h"
#include "config_loader.h"
#include "command_factory.h"
#include "action_commands.h"
#include <cctype>

ConsoleInputReader::ConsoleInputReader(const std::string& configFile)
    : config(ConfigLoader::loadConfig(configFile)) {}

std::unique_ptr<Command> ConsoleInputReader::getNextCommand() {
    std::cout << "\nEnter command: ";
    char inputChar;
    if (!(std::cin >> inputChar)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidOperationException("Failed to read player command: input stream error");
    }

    CommandFactory factory(config);
    auto command = factory.createCommand(inputChar);

    char upperCmd = std::toupper(inputChar);
    if (upperCmd == std::toupper(config.saveGameKey)) {
        std::string filename = getFilename("Enter save filename: ");
        return std::make_unique<SaveGameCommand>(filename);
    } else if (upperCmd == std::toupper(config.loadGameKey)) {
        std::string filename = getFilename("Enter load filename: ");
        return std::make_unique<LoadGameCommand>(filename);
    }

    if (std::isdigit(upperCmd) && upperCmd >= config.castSpellKey && 
        upperCmd < (char)(config.castSpellKey + 4)) {
    }

    return command;
}

GameCommand ConsoleInputReader::getNextGameCommand() {
    std::cout << "\nEnter command: ";
    char command;
    if (!(std::cin >> command)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidOperationException("Failed to read player command: input stream error");
    }

    char upperCmd = std::toupper(command);

    if (upperCmd == std::toupper(config.moveUpKey)) {
        return GameCommand(GameCommand::Type::MOVE_UP);
    } else if (upperCmd == std::toupper(config.moveDownKey)) {
        return GameCommand(GameCommand::Type::MOVE_DOWN);
    } else if (upperCmd == std::toupper(config.moveLeftKey)) {
        return GameCommand(GameCommand::Type::MOVE_LEFT);
    } else if (upperCmd == std::toupper(config.moveRightKey)) {
        return GameCommand(GameCommand::Type::MOVE_RIGHT);
    } else if (upperCmd == std::toupper(config.switchModeKey)) {
        return GameCommand(GameCommand::Type::SWITCH_MODE);
    } else if (upperCmd == std::toupper(config.rangedAttackKey)) {
        return GameCommand(GameCommand::Type::RANGED_ATTACK);
    } else if (upperCmd == std::toupper(config.saveGameKey)) {
        GameCommand cmd(GameCommand::Type::SAVE_GAME);
        cmd.filename = getFilename("Enter save filename: ");
        return cmd;
    } else if (upperCmd == std::toupper(config.loadGameKey)) {
        GameCommand cmd(GameCommand::Type::LOAD_GAME);
        cmd.filename = getFilename("Enter load filename: ");
        return cmd;
    } else if (upperCmd == std::toupper(config.quitKey)) {
        return GameCommand(GameCommand::Type::QUIT);
    } else if (std::isdigit(upperCmd) && upperCmd >= config.castSpellKey && 
               upperCmd < (char)(config.castSpellKey + 4)) {
        GameCommand cmd(GameCommand::Type::CAST_SPELL);
        cmd.spellIndex = upperCmd - config.castSpellKey;
        cmd.target = getSpellTarget();
        return cmd;
    }
    
    return GameCommand(GameCommand::Type::INVALID);
}

std::pair<int, int> ConsoleInputReader::getSpellTarget() {
    int x, y;
    std::cout << "Enter target X: ";
    if (!(std::cin >> x)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidOperationException("Failed to read spell target X coordinate: input stream error");
    }
    
    std::cout << "Enter target Y: ";
    if (!(std::cin >> y)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidOperationException("Failed to read spell target Y coordinate: input stream error");
    }
    return {x, y};
}

bool ConsoleInputReader::getYesNoChoice(const std::string& prompt) {
    std::cout << prompt;
    char choice;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidOperationException("Failed to read yes/no choice: input stream error");
    }
    return (choice == 'y' || choice == 'Y');
}

std::string ConsoleInputReader::getFilename(const std::string& prompt) {
    std::cout << prompt;
    std::string filename;
    if (!(std::cin >> filename)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidOperationException("Failed to read filename: input stream error");
    }
    if (filename.empty()) {
        throw InvalidOperationException("Filename cannot be empty");
    }
    return filename;
}

std::map<char, std::string> ConsoleInputReader::getKeyMapping() const {
    std::map<char, std::string> mapping;
    mapping[std::toupper(config.moveUpKey)] = "Move Up";
    mapping[std::toupper(config.moveDownKey)] = "Move Down";
    mapping[std::toupper(config.moveLeftKey)] = "Move Left";
    mapping[std::toupper(config.moveRightKey)] = "Move Right";
    mapping[std::toupper(config.switchModeKey)] = "Switch Mode";
    mapping[std::toupper(config.rangedAttackKey)] = "Ranged Attack";
    mapping[std::toupper(config.saveGameKey)] = "Save Game";
    mapping[std::toupper(config.loadGameKey)] = "Load Game";
    mapping[std::toupper(config.quitKey)] = "Quit";
    return mapping;
}