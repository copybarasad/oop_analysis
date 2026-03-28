#ifndef COMMAND_H
#define COMMAND_H

#include <string>

// Сущность, представляющая команду
class Command {
public:
    enum class Type {
        MOVE_UP,
        MOVE_DOWN,
        MOVE_LEFT,
        MOVE_RIGHT,
        SWITCH_MODE,
        RANGED_ATTACK,
        CAST_SPELL_1,
        CAST_SPELL_2,
        CAST_SPELL_3,
        CAST_SPELL_4,
        CAST_SPELL_5,
        SAVE_GAME,
        LOAD_GAME,
        RESTART_GAME,
        QUIT_GAME,
        INVALID
    };
    
    Command();
    Command(Type type);
    Command(Type type, int param);
    
    Type getType() const;
    int getParam() const;
    std::string getName() const;
    bool isValid() const;
    
private:
    Type type_;
    int param_;
};

#endif