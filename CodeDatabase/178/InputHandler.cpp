#include "InputHandler.h"
#include "CommandFactory.h"
#include <iostream>
#include <cctype>
#include <memory>

bool InputHandler::handleGameInput(GameController& controller, char input) {
    static CommandFactory commandFactory;
    static bool configLoaded = false;
    if (!configLoaded) {
        configLoaded = true;
    }
    auto command = commandFactory.createCommand(input, controller);
    
    if (!command) {
        return false;
    }
    command->execute(controller);
    return true;
}

void InputHandler::showHelp() {
    CommandFactory factory;
    factory.showHelp();
}

Position InputHandler::getDirectionFromInput(char input) {
    switch (std::tolower(input)) {
        case 'w': return Position(0, -1);
        case 's': return Position(0, 1);
        case 'a': return Position(-1, 0);
        case 'd': return Position(1, 0);
        default: return Position(0, 0);
    }
}