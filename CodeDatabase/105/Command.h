#pragma once

#include <string>
#include <vector>
#include <map>
#include <utility>

enum class CommandType {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    CAST_SPELL,
    SWITCH_COMBAT,
    BUY_SPELL,
    SAVE_GAME,
    SHOW_MENU,
    QUIT_GAME,
    SELECT_OPTION,
    ENTER_COORDINATES,
    ENTER_SIZE,
    CANCEL,
    CONFIRM,
    INVALID,
    UNKNOWN
};


class CommandData {
public:
    enum class Type { NONE, INT, COORDS, SPELL, SIZE };
    
private:
    Type type;
    int intValue;
    std::pair<int, int> coordsValue;
    std::pair<int, int> sizeValue;
    
    struct SpellInfo {
        int spellIndex;
        bool hasTarget;
        int targetX;
        int targetY;
        
        SpellInfo() : spellIndex(0), hasTarget(false), targetX(0), targetY(0) {}
    } spellValue;
    
public:
    CommandData() : type(Type::NONE), intValue(0) {}
    
    void setInt(int value) { 
        type = Type::INT; 
        intValue = value; 
    }
    
    void setCoords(int x, int y) { 
        type = Type::COORDS; 
        coordsValue = std::make_pair(x, y); 
    }
    
    void setSize(int width, int height) { 
        type = Type::SIZE; 
        sizeValue = std::make_pair(width, height); 
    }
    
    void setSpell(int index, bool hasTarget = false, int x = 0, int y = 0) {
        type = Type::SPELL;
        spellValue.spellIndex = index;
        spellValue.hasTarget = hasTarget;
        spellValue.targetX = x;
        spellValue.targetY = y;
    }
    
    Type getType() const { return type; }
    int getInt() const { return intValue; }
    std::pair<int, int> getCoords() const { return coordsValue; }
    std::pair<int, int> getSize() const { return sizeValue; }
    
    struct SpellResult {
        int spellIndex;
        bool hasTarget;
        int targetX;
        int targetY;
    };
    
    SpellResult getSpell() const { 
        return SpellResult{
            spellValue.spellIndex, 
            spellValue.hasTarget, 
            spellValue.targetX, 
            spellValue.targetY
        }; 
    }
};

class Command {
private:
    CommandType type;
    CommandData data;
    
public:
    // Конструкторы
    Command(CommandType type) : type(type) {}
    
    Command(CommandType type, int value) : type(type) { 
        data.setInt(value); 
    }
    
    // ТОЛЬКО для координат
    Command(CommandType type, int x, int y, bool isCoords) : type(type) {
        if (isCoords) {
            data.setCoords(x, y);
        } else {
            // Для размеров
            data.setSize(x, y);
        }
    }
    
    // для заклинаний
    Command(CommandType type, int spellIndex, bool hasTarget, int targetX, int targetY) 
        : type(type) { 
        data.setSpell(spellIndex, hasTarget, targetX, targetY); 
    }
    
    
    CommandType getType() const { return type; }
    CommandData getData() const { return data; }
    int getInt() const { return data.getInt(); }
    std::pair<int, int> getCoords() const { return data.getCoords(); }
    std::pair<int, int> getSize() const { return data.getSize(); }
    CommandData::SpellResult getSpell() const { return data.getSpell(); }
    bool hasData() const { return data.getType() != CommandData::Type::NONE; }
};

namespace CommandUtils {
    inline std::string commandTypeToString(CommandType type) {
        static const std::map<CommandType, std::string> names = {
            {CommandType::MOVE_UP, "MOVE_UP"},
            {CommandType::MOVE_DOWN, "MOVE_DOWN"},
            {CommandType::MOVE_LEFT, "MOVE_LEFT"},
            {CommandType::MOVE_RIGHT, "MOVE_RIGHT"},
            {CommandType::CAST_SPELL, "CAST_SPELL"},
            {CommandType::SWITCH_COMBAT, "SWITCH_COMBAT"},
            {CommandType::BUY_SPELL, "BUY_SPELL"},
            {CommandType::SAVE_GAME, "SAVE_GAME"},
            {CommandType::SHOW_MENU, "SHOW_MENU"},
            {CommandType::QUIT_GAME, "QUIT_GAME"},
            {CommandType::SELECT_OPTION, "SELECT_OPTION"},
            {CommandType::ENTER_COORDINATES, "ENTER_COORDINATES"},
            {CommandType::ENTER_SIZE, "ENTER_SIZE"},
            {CommandType::CANCEL, "CANCEL"},
            {CommandType::CONFIRM, "CONFIRM"},
            {CommandType::INVALID, "INVALID"},
            {CommandType::UNKNOWN, "UNKNOWN"}
        };
        auto it = names.find(type);
        return it != names.end() ? it->second : "UNKNOWN";
    }

    inline CommandType stringToCommandType(const std::string& str) {
        static const std::map<std::string, CommandType> types = {
            {"MOVE_UP", CommandType::MOVE_UP},
            {"MOVE_DOWN", CommandType::MOVE_DOWN},
            {"MOVE_LEFT", CommandType::MOVE_LEFT},
            {"MOVE_RIGHT", CommandType::MOVE_RIGHT},
            {"CAST_SPELL", CommandType::CAST_SPELL},
            {"SWITCH_COMBAT", CommandType::SWITCH_COMBAT},
            {"BUY_SPELL", CommandType::BUY_SPELL},
            {"SAVE_GAME", CommandType::SAVE_GAME},
            {"SHOW_MENU", CommandType::SHOW_MENU},
            {"QUIT_GAME", CommandType::QUIT_GAME},
            {"SELECT_OPTION", CommandType::SELECT_OPTION},
            {"ENTER_COORDINATES", CommandType::ENTER_COORDINATES},
            {"ENTER_SIZE", CommandType::ENTER_SIZE},
            {"CANCEL", CommandType::CANCEL},
            {"CONFIRM", CommandType::CONFIRM},
            {"INVALID", CommandType::INVALID},
            {"UNKNOWN", CommandType::UNKNOWN}
        };
        auto it = types.find(str);
        return it != types.end() ? it->second : CommandType::UNKNOWN;
    }

    inline std::vector<CommandType> getAllCommandTypes() {
        return {
            CommandType::MOVE_UP,
            CommandType::MOVE_DOWN,
            CommandType::MOVE_LEFT,
            CommandType::MOVE_RIGHT,
            CommandType::CAST_SPELL,
            CommandType::SWITCH_COMBAT,
            CommandType::BUY_SPELL,
            CommandType::SAVE_GAME,
            CommandType::SHOW_MENU,
            CommandType::QUIT_GAME
        };
    }
}