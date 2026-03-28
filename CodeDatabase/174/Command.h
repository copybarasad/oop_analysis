#ifndef COMMAND_H
#define COMMAND_H

enum class CommandType {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    ATTACK,
    CAST_SPELL,
    CHANGE_STYLE,
    SAVE_GAME,
    LOAD_GAME,
    QUIT,
    UNKNOWN
};

class Command {
public:
    Command(CommandType type) : type_(type) {}
    
    CommandType getType() const { return type_; }
    
private:
    CommandType type_;
};

#endif