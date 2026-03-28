#ifndef GAME_COMMAND_H
#define GAME_COMMAND_H
#include <string>

class GameCommand {
public:
    enum class Type {
        MOVE_UP,
        MOVE_DOWN,
        MOVE_LEFT,
        MOVE_RIGHT,
        CAST_SPELL,
        TOGGLE_COMBAT,
        SAVE_GAME,
        QUIT,
        MENU_SELECT_1,
        MENU_SELECT_2,
        MENU_SELECT_3,
        SPELL_0,
        SPELL_1,
        SPELL_2,
        SPELL_3,
        SPELL_4,
        SPELL_5,
        SPELL_6,
        SPELL_7,
        SPELL_8,
        SPELL_9,
        CONFIRM,
        CANCEL,
        UNKNOWN
    };
    
private:
    Type type;
    char data; 
    
public:
    GameCommand(Type t = Type::UNKNOWN, char d = '\0') 
        : type(t), data(d) {}
    
    Type getType() const { return type; }
    char getData() const { return data; }
    int getDataAsInt() const { 
        if (data >= '0' && data <= '9') {
            return data - '0';
        }
        return 0;
    }
};

#endif