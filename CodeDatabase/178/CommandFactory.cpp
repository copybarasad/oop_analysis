#include "CommandFactory.h"
#include "InputHandler.h"
#include <cctype>
#include <iostream>
#include <limits>

CommandFactory::CommandFactory() {
    config = InputConfig::createDefault();
}

void CommandFactory::loadConfig(const std::string& filename) {
    try {
        config = InputConfigLoader::load(filename);
        std::cout << "Configuration loaded from: " << filename << std::endl;
        config.printConfig();
    } catch (const std::exception& e) {
        std::cerr << "Error loading config: " << e.what() << std::endl;
        std::cout << "Using default configuration." << std::endl;
        config = InputConfig::createDefault();
    }
}

std::unique_ptr<ICommand> CommandFactory::createCommand(char input, GameController& controller) {
    input = std::tolower(input);
    
    try {
        CommandType type = config.getCommandForKey(input);
        return createCommandFromType(type, controller);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return nullptr;
    }
}

std::unique_ptr<ICommand> CommandFactory::createCommandFromType(CommandType type, GameController& controller) {
    switch (type) {
        case CommandType::MOVE_UP:
            return std::make_unique<MoveCommand>(Position(0, -1));
        case CommandType::MOVE_DOWN:
            return std::make_unique<MoveCommand>(Position(0, 1));
        case CommandType::MOVE_LEFT:
            return std::make_unique<MoveCommand>(Position(-1, 0));
        case CommandType::MOVE_RIGHT:
            return std::make_unique<MoveCommand>(Position(1, 0));
        case CommandType::ATTACK:
            return createAttackCommand(controller);
        case CommandType::AREA_ATTACK:
            return std::make_unique<AreaAttackCommand>();
        case CommandType::SWITCH_COMBAT:
            return std::make_unique<SwitchCombatCommand>();
        case CommandType::LIST_SPELLS:
            return std::make_unique<ListSpellsCommand>();
        case CommandType::USE_SPELL:
            return createUseSpellCommand(controller);
        case CommandType::BUY_SPELL:
            return std::make_unique<BuySpellCommand>();
        case CommandType::UPGRADE_SPELL:
            return createUpgradeSpellCommand(controller);
        case CommandType::HELP:
            return std::make_unique<HelpCommand>();
        case CommandType::SAVE:
            return std::make_unique<SaveGameCommand>();
        case CommandType::QUIT:
            return std::make_unique<QuitCommand>();
        default:
            return nullptr;
    }
}

std::unique_ptr<ICommand> CommandFactory::createAttackCommand(GameController& controller) {
    std::cout << "Enter attack direction (WASD): ";
    char dir;
    std::cin >> dir;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    Position direction = InputHandler::getDirectionFromInput(dir);
    return std::make_unique<AttackCommand>(direction);
}

std::unique_ptr<ICommand> CommandFactory::createUseSpellCommand(GameController& controller) {
    int idx, tx, ty;
    std::cout << "Enter spell index: ";
    if (std::cin >> idx) {
        std::cout << "Enter target x y: ";
        if (std::cin >> tx >> ty) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return std::make_unique<UseSpellCommand>(idx, Position(tx, ty));
        }
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return nullptr;
}

std::unique_ptr<ICommand> CommandFactory::createUpgradeSpellCommand(GameController& controller) {
    int idx;
    std::cout << "Enter spell index to upgrade: ";
    if (std::cin >> idx) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return std::make_unique<UpgradeSpellCommand>(idx);
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return nullptr;
}

bool CommandFactory::isValidInput(char input) const {
    input = std::tolower(input);
    return config.hasKey(input);
}

void CommandFactory::showHelp() const {
    std::cout << "\n=== GAME CONTROLS ===" << std::endl;
    std::cout << "Based on configuration file" << std::endl;
    std::cout << "=====================" << std::endl;
    config.printConfig();
}

void CommandFactory::printConfig() const {
    config.printConfig();
}