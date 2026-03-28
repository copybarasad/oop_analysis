#include "command_factory.h"
#include "movement_commands.h"
#include "action_commands.h"
#include <cctype>

std::unique_ptr<Command> CommandFactory::createCommand(char inputChar) {
    char upperCmd = std::toupper(inputChar);

    if (upperCmd == std::toupper(config.moveUpKey)) {
        return std::make_unique<MoveUpCommand>();
    } else if (upperCmd == std::toupper(config.moveDownKey)) {
        return std::make_unique<MoveDownCommand>();
    } else if (upperCmd == std::toupper(config.moveLeftKey)) {
        return std::make_unique<MoveLeftCommand>();
    } else if (upperCmd == std::toupper(config.moveRightKey)) {
        return std::make_unique<MoveRightCommand>();
    }
    else if (upperCmd == std::toupper(config.switchModeKey)) {
        return std::make_unique<SwitchModeCommand>();
    } else if (upperCmd == std::toupper(config.rangedAttackKey)) {
        return std::make_unique<RangedAttackCommand>();
    } else if (upperCmd == std::toupper(config.saveGameKey)) {
        return std::make_unique<InvalidCommand>();
    } else if (upperCmd == std::toupper(config.loadGameKey)) {
        return std::make_unique<InvalidCommand>();
    } else if (upperCmd == std::toupper(config.quitKey)) {
        return std::make_unique<QuitCommand>();
    }
    else if (std::isdigit(upperCmd) && upperCmd >= config.castSpellKey && 
             upperCmd < (char)(config.castSpellKey + 4)) {
        int spellIndex = upperCmd - config.castSpellKey;
        return std::make_unique<CastSpellCommand>(spellIndex);
    }

    return std::make_unique<InvalidCommand>();
}