#include "../include/InputHandler.h"
#include "../include/SpellHand.h"
#include "../include/Spell.h"
#include "../include/KeyBindingConfig.h"
#include <iostream>
#include <limits>
#include <string>
#include <cctype>

PlayerAction InputHandler::getPlayerAction(const SpellHand* spellHand) {
    const auto& config = KeyBindingConfig::getInstance();
    
    char moveUp = 'w', moveDown = 's', moveLeft = 'a', moveRight = 'd';
    auto bindings = config.getAllBindings();
    for (const auto& [key, desc] : bindings) {
        if (desc.find("MOVE UP") != std::string::npos) moveUp = key;
        else if (desc.find("MOVE DOWN") != std::string::npos) moveDown = key;
        else if (desc.find("MOVE LEFT") != std::string::npos) moveLeft = key;
        else if (desc.find("MOVE RIGHT") != std::string::npos) moveRight = key;
    }
    
    char attackKey = config.getKeyForCommand(CommandType::ATTACK);
    char modeKey = config.getKeyForCommand(CommandType::SWITCH_MODE);
    char castKey = config.getKeyForCommand(CommandType::CAST_SPELL);
    char buyKey = config.getKeyForCommand(CommandType::BUY_SPELL);
    char skipKey = config.getKeyForCommand(CommandType::SKIP_TURN);
    char restartKey = config.getKeyForCommand(CommandType::RESTART);
    char quitKey = config.getKeyForCommand(CommandType::QUIT);
    
    std::cout << "\nActions:\n";
    std::cout << "1. Move (" << moveUp << "/" << moveLeft << "/" << moveDown << "/" << moveRight << " for up/left/down/right)\n";
    std::cout << "2. Attack (" << attackKey << " + direction)\n";
    std::cout << "3. Switch combat mode (" << modeKey << ")\n";
    std::cout << "4. Cast spell (" << castKey << ")\n";
    std::cout << "5. Buy spell (" << buyKey << ") - 50 points\n";
    std::cout << "6. Save game (SAVE)\n";
    std::cout << "7. Load game (LOAD)\n";
    if (skipKey == ' ' || skipKey == '.') {
        std::cout << "8. Skip turn (" << (skipKey == ' ' ? "SPACE" : ".") << ")\n";
    } else {
        std::cout << "8. Skip turn (" << skipKey << ")\n";
    }
    std::cout << "9. Restart (" << restartKey << ")\n";
    std::cout << "10. Quit (" << quitKey << ")\n";
    std::cout << "Enter action: ";
    
    std::string input;
    std::getline(std::cin, input);
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input.\n";
        return PlayerAction(ActionType::INVALID);
    }
    
    if (input.empty()) {
        std::cout << "Error: Empty input. Please enter an action.\n";
        return PlayerAction(ActionType::INVALID);
    }
    
    if (input == "space" || input == "SPACE") {
        return PlayerAction(ActionType::SKIP_TURN);
    }
    
    if (input == "Save" || input == "SAVE" || input == "save") {
        return PlayerAction(ActionType::SAVE_GAME);
    }
    
    if (input == "Load" || input == "LOAD" || input == "load") {
        return PlayerAction(ActionType::LOAD_GAME);
    }
    
    if (input.length() > 1) {
        std::cout << "Error: Please enter exactly ONE command character.\n";
        return PlayerAction(ActionType::INVALID);
    }
    
    char action = input[0];
    
    if (action == quitKey || action == std::toupper(quitKey)) {
        return PlayerAction(ActionType::QUIT);
    }
    
    if (action == restartKey || action == std::toupper(restartKey)) {
        return PlayerAction(ActionType::RESTART);
    }
    
    if (action == modeKey || action == std::toupper(modeKey)) {
        return PlayerAction(ActionType::SWITCH_MODE);
    }
    
    if (action == skipKey || action == ' ') {
        return PlayerAction(ActionType::SKIP_TURN);
    }
    
    if (action == buyKey || action == std::toupper(buyKey)) {
        return PlayerAction(ActionType::BUY_SPELL);
    }
    
    if (action == castKey || action == std::toupper(castKey)) {
        std::cout << "Spell index (0-4): ";
        std::string spellIndexStr;
        std::getline(std::cin, spellIndexStr);
        
        if (spellIndexStr.empty() || spellIndexStr.length() != 1 || !isdigit(spellIndexStr[0])) {
            std::cout << "Error: Invalid spell index.\n";
            return PlayerAction(ActionType::INVALID);
        }
        
        int spellIndex = spellIndexStr[0] - '0';
        
        bool needsTarget = true;
        if (spellHand != nullptr && spellHand->hasSpell(spellIndex)) {
            const Spell* spell = spellHand->getSpell(spellIndex);
            if (spell->getName() == "Empower" || spell->getName() == "Summon") {
                needsTarget = false;
            }
        }
        
        if (!needsTarget) {
            return PlayerAction(ActionType::CAST_SPELL, Direction::NONE, spellIndex, Position(1, 1));
        }
        
        std::cout << "Target X (column): ";
        std::string xStr;
        std::getline(std::cin, xStr);
        
        std::cout << "Target Y (row): ";
        std::string yStr;
        std::getline(std::cin, yStr);
        
        try {
            int x = std::stoi(xStr);
            int y = std::stoi(yStr);
            
            return PlayerAction(ActionType::CAST_SPELL, Direction::NONE, spellIndex, Position(x, y));
        } catch (...) {
            std::cout << "Error: Invalid target coordinates.\n";
            return PlayerAction(ActionType::INVALID);
        }
    }
    
    if (action == attackKey || action == std::toupper(attackKey)) {
        std::cout << "Attack direction (" << moveUp << "/" << moveLeft << "/" << moveDown << "/" << moveRight << "): ";
        Direction attackDir = parseDirectionInput();
        if (attackDir == Direction::NONE) {
            return PlayerAction(ActionType::INVALID);
        }
        return PlayerAction(ActionType::ATTACK, attackDir);
    }
    
    Direction moveDir = Direction::NONE;
    if (action == moveUp || action == std::toupper(moveUp)) {
        moveDir = Direction::UP;
    } else if (action == moveDown || action == std::toupper(moveDown)) {
        moveDir = Direction::DOWN;
    } else if (action == moveLeft || action == std::toupper(moveLeft)) {
        moveDir = Direction::LEFT;
    } else if (action == moveRight || action == std::toupper(moveRight)) {
        moveDir = Direction::RIGHT;
    } else {
        std::cout << "Error: Invalid action.\n";
        return PlayerAction(ActionType::INVALID);
    }
    
    return PlayerAction(ActionType::MOVE, moveDir);
}

Direction InputHandler::parseDirectionInput() {
    const auto& config = KeyBindingConfig::getInstance();
    
    char moveUp = 'w', moveDown = 's', moveLeft = 'a', moveRight = 'd';
    auto bindings = config.getAllBindings();
    for (const auto& [key, desc] : bindings) {
        if (desc.find("MOVE UP") != std::string::npos) moveUp = key;
        else if (desc.find("MOVE DOWN") != std::string::npos) moveDown = key;
        else if (desc.find("MOVE LEFT") != std::string::npos) moveLeft = key;
        else if (desc.find("MOVE RIGHT") != std::string::npos) moveRight = key;
    }
    
    std::string input;
    std::getline(std::cin, input);
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input.\n";
        return Direction::NONE;
    }
    
    if (input.empty()) {
        std::cout << "Error: Empty input. Please enter a direction.\n";
        return Direction::NONE;
    }
    
    if (input.length() > 1) {
        std::cout << "Error: Please enter exactly ONE direction character.\n";
        return Direction::NONE;
    }
    
    char dirChar = input[0];
    
    if (dirChar == moveUp || dirChar == std::toupper(moveUp)) {
        return Direction::UP;
    } else if (dirChar == moveDown || dirChar == std::toupper(moveDown)) {
        return Direction::DOWN;
    } else if (dirChar == moveLeft || dirChar == std::toupper(moveLeft)) {
        return Direction::LEFT;
    } else if (dirChar == moveRight || dirChar == std::toupper(moveRight)) {
        return Direction::RIGHT;
    } else {
        std::cout << "Error: Invalid direction. Please use " << moveUp << "/" << moveLeft << "/" << moveDown << "/" << moveRight << ".\n";
        return Direction::NONE;
    }
}
