#pragma once
#include <string>
#include <iostream>
#include <limits>
#include <map>
#include <functional>

class InputHandler 
{
public:
    enum class CommandType 
    {
        MOVE_UP,
        MOVE_DOWN, 
        MOVE_LEFT,
        MOVE_RIGHT,
        SHOW_STATS,
        SHOW_INSTRUCTIONS,
        SHOW_SPELLS,
        BUY_SPELLS,
        CAST_SPELL,
        CHANGE_COMBAT_RANGE,
        SAVE_GAME,
        LOAD_GAME,
        EXIT,
        INVALID
    };

    struct CommandResult 
    {
        CommandType type;
        std::string message;
        int spellIndex;
        int combatRange;
        
        CommandResult(CommandType t, const std::string& msg = "", int spellIdx = -1, int range = -1)
            : type(t), message(msg), spellIndex(spellIdx), combatRange(range) {}
    };

private:
    std::map<char, CommandType> commandMap;
    void initializeCommandMap();

public:
    InputHandler();
    CommandResult handleInput(char input);
    
    CommandResult handleSpellInput();
    CommandResult handleCombatRangeInput();
    CommandResult handleMenuInput();
    
    int getIntegerInput(const std::string& prompt = "");
    bool getYesNoInput(const std::string& prompt = "");
    std::string getStringInput(const std::string& prompt = "");
    
    static std::string commandTypeToString(CommandType type);
};




