#ifndef COMMAND_H
#define COMMAND_H

#include <string>

// Типы команд для игры
enum class CmdType {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    SWITCH_MODE,
    USE_SPELL_1,
    USE_SPELL_2,
    SAVE_GAME,
    LOAD_GAME,
    MENU_SELECT_1,
    MENU_SELECT_2,
    MENU_SELECT_3,
    QUIT_GAME,
    CANCEL_SPELL,
    UNKNOWN
};

// Класс команды, представляющий действие в игре
class Command {
private:
    CmdType type;
    std::string description;
    
public:
    Command(CmdType cmdType = CmdType::UNKNOWN, const std::string& desc = "")
        : type(cmdType), description(desc) {}
    
    CmdType getType() const { return type; }
    std::string getDescription() const { return description; }
    bool isValid() const { return type != CmdType::UNKNOWN; }
};

#endif