#include "InputHandler.h"

using namespace std;

InputHandler::InputHandler() 
{
    initializeCommandMap();
}

void InputHandler::initializeCommandMap() 
{
    commandMap = 
    {
        {'w', CommandType::MOVE_UP},
        {'s', CommandType::MOVE_DOWN},
        {'a', CommandType::MOVE_LEFT},
        {'d', CommandType::MOVE_RIGHT},
        {'z', CommandType::SHOW_STATS},
        {'i', CommandType::SHOW_INSTRUCTIONS},
        {'h', CommandType::SHOW_SPELLS},
        {'p', CommandType::BUY_SPELLS},
        {'c', CommandType::CAST_SPELL},
        {'b', CommandType::CHANGE_COMBAT_RANGE},
        {'v', CommandType::SAVE_GAME},
        {'l', CommandType::LOAD_GAME},
        {'e', CommandType::EXIT}
    };
}

InputHandler::CommandResult InputHandler::handleInput(char input) 
{
    char lowerInput = tolower(input);
    auto it = commandMap.find(lowerInput);
    
    if (it != commandMap.end()) 
    {
        CommandType type = it->second;

        if (type == CommandType::CAST_SPELL) 
        {
            return handleSpellInput();
        } 
        else if (type == CommandType::CHANGE_COMBAT_RANGE) 
        {
            return handleCombatRangeInput();
        } 
        else 
        {
            return CommandResult(type);
        }
    } 
    else 
    {
        return CommandResult(CommandType::INVALID, "There is no such command");
    }
}

InputHandler::CommandResult InputHandler::handleSpellInput() 
{
    cout << "Enter spell number to cast (or -1 to cancel): ";
    int spellIndex = getIntegerInput();
    
    if (spellIndex == -1) 
    {
        return CommandResult(CommandType::INVALID, "Spell casting cancelled");
    }
    
    return CommandResult(CommandType::CAST_SPELL, "", spellIndex);
}

InputHandler::CommandResult InputHandler::handleCombatRangeInput() 
{
    cout << "To change the damage, press 0 or 1" << endl;
    cout << "0 - close combat, 1 - long-range combat" << endl;
    cout << "Enter the damage range value:" << endl;
    
    int combat = getIntegerInput();
    
    if (combat != 1 && combat != 0) 
    {
        cout << "You have entered an incorrect range value, the default range value is 0" << endl;
        combat = 0;
    }
    
    return CommandResult(CommandType::CHANGE_COMBAT_RANGE, "", -1, combat);
}

InputHandler::CommandResult InputHandler::handleMenuInput() 
{
    cout << "=== Main Menu ===" << endl;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "Choose: ";
    
    int choice = getIntegerInput();
    
    if (choice == 1) 
    {
        return CommandResult(CommandType::MOVE_UP, "NEW_GAME"); // Используем MOVE_UP как заглушку для нового game
    } 
    else if (choice == 2) 
    {
        return CommandResult(CommandType::MOVE_DOWN, "LOAD_GAME"); // Используем MOVE_DOWN как заглушку для загрузки
    } 
    else 
    {
        return CommandResult(CommandType::INVALID, "Invalid menu choice");
    }
}

int InputHandler::getIntegerInput(const std::string& prompt) 
{
    if (!prompt.empty()) 
    {
        cout << prompt;
    }
    
    int value;
    while (!(cin >> value)) 
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return value;
}

bool InputHandler::getYesNoInput(const std::string& prompt) 
{
    if (!prompt.empty()) 
    {
        cout << prompt;
    }
    
    char choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return tolower(choice) == 'y';
}

std::string InputHandler::getStringInput(const std::string& prompt) 
{
    if (!prompt.empty()) 
    {
        cout << prompt;
    }
    
    std::string input;
    getline(cin, input);
    return input;
}

std::string InputHandler::commandTypeToString(CommandType type) 
{
    switch (type) 
    {
        case CommandType::MOVE_UP: return "MOVE_UP";
        case CommandType::MOVE_DOWN: return "MOVE_DOWN";
        case CommandType::MOVE_LEFT: return "MOVE_LEFT";
        case CommandType::MOVE_RIGHT: return "MOVE_RIGHT";
        case CommandType::SHOW_STATS: return "SHOW_STATS";
        case CommandType::SHOW_INSTRUCTIONS: return "SHOW_INSTRUCTIONS";
        case CommandType::SHOW_SPELLS: return "SHOW_SPELLS";
        case CommandType::BUY_SPELLS: return "BUY_SPELLS";
        case CommandType::CAST_SPELL: return "CAST_SPELL";
        case CommandType::CHANGE_COMBAT_RANGE: return "CHANGE_COMBAT_RANGE";
        case CommandType::SAVE_GAME: return "SAVE_GAME";
        case CommandType::LOAD_GAME: return "LOAD_GAME";
        case CommandType::EXIT: return "EXIT";
        case CommandType::INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}