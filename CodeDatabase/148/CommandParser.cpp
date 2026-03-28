#include "CommandParser.h"
#include "MoveCommand.h"
#include "CastSpellCommand.h"
#include "SwitchModeCommand.h"
#include "SaveGameCommand.h"
#include "LoadGameCommand.h"
#include "QuitCommand.h"
#include "ShowStatusCommand.h"
#include "HelpCommand.h"
#include "GameTypes.h"
#include <algorithm>
#include <cctype>
#include <sstream>


/**
 * @file CommandParser.cpp
 * @brief Реализация парсера команд
 */


/**
 * @brief Парсить строку в команду с учетом конфигурации клавиш
 * @param input Строка пользовательского ввода
 * @param keyConfig Конфигурация привязки клавиш
 * @return Умный указатель на команду или nullptr
 */
std::unique_ptr<Command> CommandParser::parse(const std::string& input,
                                               const KeyBindingConfig& keyConfig) {
    if (input.empty()) {
        return nullptr;
    }
    
    // Убираем пробелы и приводим к верхнему регистру
    std::string trimmedInput = trim(input);
    std::string upperInput = toUpperCase(trimmedInput);
    
    // Сначала проверяем составные команды (CAST, SAVE, LOAD)
    std::istringstream iss(upperInput);
    std::string firstWord;
    iss >> firstWord;
    
    // Команда заклинания: "CAST 0" или по привязке "C 0"
    const CommandType* castCmd = keyConfig.getCommand(firstWord);
    if ((castCmd && *castCmd == CommandType::CAST_SPELL) || firstWord == "CAST") {
        int spellIndex;
        if (iss >> spellIndex) {
            return std::make_unique<CastSpellCommand>(spellIndex);
        }
        return nullptr;
    }
    
    // Команда сохранения: "SAVE quicksave" или по привязке
    const CommandType* saveCmd = keyConfig.getCommand(firstWord);
    if ((saveCmd && *saveCmd == CommandType::SAVE_GAME) || firstWord == "SAVE") {
        std::string saveName;
        if (iss >> saveName) {
            std::transform(saveName.begin(), saveName.end(), saveName.begin(), ::tolower);
            return std::make_unique<SaveGameCommand>(saveName);
        }
        return nullptr;
    }
    
    // Команда загрузки: "LOAD quicksave" или по привязке
    const CommandType* loadCmd = keyConfig.getCommand(firstWord);
    if ((loadCmd && *loadCmd == CommandType::LOAD_GAME) || firstWord == "LOAD") {
        std::string saveName;
        if (iss >> saveName) {
            std::transform(saveName.begin(), saveName.end(), saveName.begin(), ::tolower);
            return std::make_unique<LoadGameCommand>(saveName);
        }
        return nullptr;
    }
    
    // Проверяем одиночные команды через конфигурацию
    const CommandType* cmdType = keyConfig.getCommand(upperInput);
    
    if (cmdType) {
        switch (*cmdType) {
            case CommandType::MOVE_UP:
                return std::make_unique<MoveCommand>(Direction::UP);
            
            case CommandType::MOVE_DOWN:
                return std::make_unique<MoveCommand>(Direction::DOWN);
            
            case CommandType::MOVE_LEFT:
                return std::make_unique<MoveCommand>(Direction::LEFT);
            
            case CommandType::MOVE_RIGHT:
                return std::make_unique<MoveCommand>(Direction::RIGHT);
            
            case CommandType::CAST_SPELL:
                // Обрабатывается интерактивно в GameController
                return nullptr;
            
            case CommandType::SWITCH_MODE:
                return std::make_unique<SwitchModeCommand>();
            
            case CommandType::SHOW_INFO:
                return std::make_unique<ShowStatusCommand>();
            
            case CommandType::SHOW_HELP:
                return std::make_unique<HelpCommand>(&keyConfig);  // ← ИЗМЕНЕНО
            
            case CommandType::QUIT:
                return std::make_unique<QuitCommand>();
            
            case CommandType::SAVE_GAME:
            case CommandType::LOAD_GAME:
                // Эти требуют аргументы, обработаны выше
                return nullptr;
        }
    }
    
    // Альтернативные текстовые варианты команд (для совместимости)
    if (upperInput == "HELP") {
        return std::make_unique<HelpCommand>(&keyConfig);  // ← ИЗМЕНЕНО
    }
    
    if (upperInput == "INFO" || upperInput == "STATUS") {
        return std::make_unique<ShowStatusCommand>();
    }
    
    if (upperInput == "QUIT" || upperInput == "EXIT") {
        return std::make_unique<QuitCommand>();
    }
    
    if (upperInput == "SWITCH" || upperInput == "MODE") {
        return std::make_unique<SwitchModeCommand>();
    }
    
    // Команда не распознана
    return nullptr;
}


/**
 * @brief Получить справку по командам (дефолт)
 * @return Строка с описанием всех команд
 */
std::string CommandParser::getHelp() {
    return R"(
=== AVAILABLE COMMANDS ===

MOVEMENT:
  W - Move UP
  A - Move LEFT
  S - Move DOWN
  D - Move RIGHT
  (Can be customized in config/keybindings.txt)

COMBAT:
  M - Switch attack Mode (MELEE <-> RANGED)

SPELLS:
  C - Select and cast spell (interactive)
  C <index> - Cast spell directly (e.g., "C 0")
  CAST <index> - Alternative spell casting command

INFORMATION:
  I - Show player Info/status
  INFO / STATUS - Show detailed game status
  H - Show this Help message
  HELP - Show this Help message

GAME CONTROL:
  SAVE <name> - Save game (e.g., "SAVE quicksave")
  LOAD <name> - Load game (e.g., "LOAD quicksave")
  Q - Quit game
  QUIT / EXIT - Quit game

NOTES:
  - Commands are case-insensitive
  - Press ENTER after typing command
  - Key bindings can be customized in config/keybindings.txt
========================
)";
}


/**
 * @brief Получить справку с динамическими клавишами из конфига
 * @param keyConfig Конфигурация клавиш
 * @return Строка с описанием команд с реальными привязками
 */
std::string CommandParser::getHelp(const KeyBindingConfig& keyConfig) {
    std::ostringstream help;
    
    help << "\n=== AVAILABLE COMMANDS ===\n\n";
    
    help << "MOVEMENT:\n";
    help << "  " << keyConfig.getKey(CommandType::MOVE_UP) << " - Move UP\n";
    help << "  " << keyConfig.getKey(CommandType::MOVE_LEFT) << " - Move LEFT\n";
    help << "  " << keyConfig.getKey(CommandType::MOVE_DOWN) << " - Move DOWN\n";
    help << "  " << keyConfig.getKey(CommandType::MOVE_RIGHT) << " - Move RIGHT\n\n";
    
    help << "COMBAT:\n";
    help << "  " << keyConfig.getKey(CommandType::SWITCH_MODE) << " - Switch Mode (MELEE/RANGED)\n\n";
    
    help << "SPELLS:\n";
    help << "  " << keyConfig.getKey(CommandType::CAST_SPELL) << " - Cast spell (interactive)\n\n";
    
    help << "INFORMATION:\n";
    help << "  " << keyConfig.getKey(CommandType::SHOW_INFO) << " - Show status\n";
    help << "  " << keyConfig.getKey(CommandType::SHOW_HELP) << " - Show this help\n\n";
    
    help << "GAME CONTROL:\n";
    help << "  " << keyConfig.getKey(CommandType::SAVE_GAME) << " <name> - Save game\n";
    help << "  " << keyConfig.getKey(CommandType::LOAD_GAME) << " <name> - Load game\n";
    help << "  " << keyConfig.getKey(CommandType::QUIT) << " - Quit\n\n";
    
    help << "NOTES:\n";
    help << "  - Key bindings loaded from config/keybindings.txt\n";
    help << "  - Commands are case-insensitive\n";
    help << "========================\n";
    
    return help.str();
}


/**
 * @brief Преобразовать строку в верхний регистр
 * @param str Исходная строка
 * @return Строка в верхнем регистре
 */
std::string CommandParser::toUpperCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), 
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}


/**
 * @brief Убрать пробелы в начале и конце строки
 * @param str Исходная строка
 * @return Строка без пробелов по краям
 */
std::string CommandParser::trim(const std::string& str) {
    size_t start = 0;
    size_t end = str.length();
    
    // Находим первый непробельный символ
    while (start < end && std::isspace(static_cast<unsigned char>(str[start]))) {
        start++;
    }
    
    // Находим последний непробельный символ
    while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1]))) {
        end--;
    }
    
    return str.substr(start, end - start);
}


