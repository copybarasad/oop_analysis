#include "command_factory.h"
#include "game_enums.h"
#include "move_command.h"
#include "switch_mode_command.h"
#include "cast_spell_command.h"
#include "buy_spell_command.h"
#include "open_menu_command.h"
#include "quit_game_command.h"
#include "start_new_game_command.h"     
#include "open_load_menu_command.h"      
#include "back_to_main_menu_command.h"  
#include "load_selected_save_command.h"
#include "save_game_command.h"
#include "continue_game_command.h"
#include "invalid_command.h"
#include "position.h"
#include "command.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <limits>

CommandFactory::CommandFactory(std::istream& input, std::ostream& output)
    : inputStream(input), outputStream(output) {}

std::string CommandFactory::readLine() {
    std::string line;
    std::getline(inputStream, line);
    return line;
}

std::optional<int> CommandFactory::readInt(const std::string& prompt, int minValue, int maxValue) {
    outputStream << prompt;
    std::string line = readLine();
    line = stringUtils.trim(line);
    
    std::istringstream iss(line);
    int value;
    if (!(iss >> value)) {
        return std::nullopt;
    }
    if (value < minValue || value > maxValue) {
        return std::nullopt;
    }
    return value;
}

std::optional<Position> CommandFactory::readPosition(const std::string& prompt) {
    outputStream << prompt;
    std::string line = readLine();
    line = stringUtils.trim(line);
    
    std::istringstream iss(line);
    int x, y;
    if (iss >> x >> y) {
        return Position(x, y);
    }
    return std::nullopt;
}

// Хардкод движений (WASD)
Direction CommandFactory::getMovementDirection(char key) const {
    key = std::tolower(key);
    
    switch (key) {
        case 'w': return Direction::UP;
        case 's': return Direction::DOWN;
        case 'a': return Direction::LEFT;
        case 'd': return Direction::RIGHT;
        default: return Direction::UP; // или бросить исключение
    }
}

// Хардкод игровых команд
CommandType CommandFactory::getInGameCommandType(char key) const {
    key = std::tolower(key);
    
    switch (key) {
        case 'm': return CommandType::SwitchMode;
        case 'c': return CommandType::CastSpell;
        case 'b': return CommandType::BuySpell;
        case 'p': return CommandType::OpenInGameMenu;
        case 'q': return CommandType::QuitGame;
        default: return CommandType::Invalid;
    }
}

// Хардкод команд меню
CommandType CommandFactory::getMenuCommandType(char key) const {
    key = std::tolower(key);
    
    switch (key) {
        case 's': return CommandType::SaveGame;
        case 'm': return CommandType::ReturnToMainMenu;
        case 'c': return CommandType::ContinueGame;
        default: return CommandType::Invalid;
    }
}

//Основной метод для игровых команд
CommandPtr CommandFactory::createCommandFromInput(const std::string& input, const GameController& game) {
    std::string trimmed = stringUtils.trim(input);
    if (trimmed.empty()) {
        return std::make_unique<InvalidCommand>("Empty command!");
    }
    
    char key = std::tolower(trimmed[0]);
    std::string remaining = trimmed.substr(1);
    remaining = stringUtils.trim(remaining);
    
    // Проверяем движение (WASD)
    if (key == 'w' || key == 's' || key == 'a' || key == 'd') {
        Direction direction = getMovementDirection(key);
        return std::make_unique<MoveCommand>(direction);
    }
    
    // Проверяем остальные команды
    CommandType cmdType = getInGameCommandType(key);
    
    switch (cmdType) {
        case CommandType::SwitchMode:
            return std::make_unique<SwitchModeCommand>();
            
        case CommandType::BuySpell:
            return std::make_unique<BuySpellCommand>();
            
        case CommandType::OpenInGameMenu:
            return std::make_unique<OpenMenuCommand>();
            
        case CommandType::QuitGame:
            return std::make_unique<QuitGameCommand>();
            
        case CommandType::CastSpell: {
            std::istringstream iss(remaining);
            int spellIndex, x, y;
            
            if (iss >> spellIndex >> x >> y) {
                const Player& player = game.getPlayer();
                if (spellIndex >= 0 && spellIndex < player.getSpellHand().getSpellCount()) {
                    return std::make_unique<CastSpellCommand>(spellIndex, Position(x, y));
                }
            }
            
            const Player& player = game.getPlayer();
            int spellCount = player.getSpellHand().getSpellCount();
            if (spellCount <= 0) {
                return std::make_unique<InvalidCommand>("No spells available!");
            }
            
            auto spellIdxOpt = readInt("Select spell (0-" + std::to_string(spellCount - 1) + "): ", 0, spellCount - 1);
            if (!spellIdxOpt) {
                return std::make_unique<InvalidCommand>("Invalid spell selection.");
            }
            
            auto posOpt = readPosition("Enter target X Y: ");
            if (!posOpt) {
                return std::make_unique<InvalidCommand>("Invalid target coordinates.");
            }
            
            return std::make_unique<CastSpellCommand>(*spellIdxOpt, *posOpt);
        }
            
        case CommandType::Invalid:
        default:
            return std::make_unique<InvalidCommand>("Invalid command!");
    }
}

//Для команд главного меню
CommandPtr CommandFactory::createMainMenuCommand(const std::string& input) {
    std::string trimmed = stringUtils.trim(input);
    
    if (trimmed == "1") {
        return std::make_unique<StartNewGameCommand>();
    } else if (trimmed == "2") {
        return std::make_unique<OpenLoadMenuCommand>();
    } else if (trimmed == "3") {
        return std::make_unique<QuitGameCommand>();
    }
    
    return std::make_unique<InvalidCommand>("Invalid option!");
}

//Для меню загрузки
CommandPtr CommandFactory::createLoadMenuCommand(const std::string& input, int saveFilesCount) {
    std::string trimmed = stringUtils.trim(input);
    std::istringstream iss(trimmed);
    int choice;
    
    if (!(iss >> choice)) {
        return std::make_unique<InvalidCommand>("Invalid selection.");
    }
    
    if (choice == 0) {
        return std::make_unique<BackToMainMenuCommand>();
    }
    
    if (choice >= 1 && choice <= saveFilesCount) {
        return std::make_unique<LoadSelectedSaveCommand>(choice - 1);
    }
    
    return std::make_unique<InvalidCommand>("Invalid selection.");
}

//Для меню паузы
CommandPtr CommandFactory::createInGameMenuCommand(const std::string& input) {
    std::string trimmed = stringUtils.trim(input);
    if (trimmed.empty()) {
        return std::make_unique<InvalidCommand>("Invalid option!");
    }
    
    char key = std::tolower(trimmed[0]);
    CommandType cmdType = getMenuCommandType(key);
    
    switch (cmdType) {
        case CommandType::SaveGame:
            return std::make_unique<SaveGameCommand>();
            
        case CommandType::ReturnToMainMenu:
            return std::make_unique<QuitGameCommand>();
            
        case CommandType::ContinueGame:
            return std::make_unique<ContinueGameCommand>();
            
        case CommandType::Invalid:
        default:
            return std::make_unique<InvalidCommand>("Invalid menu option!");
    }
}