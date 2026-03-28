#pragma once

enum class CommandType {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    USE_SPELL,
    OPEN_SHOP,
    SAVE_GAME,
    QUIT,
    UNKNOWN 
};

class Command {
public:
    explicit Command(CommandType type);
    CommandType getType() const;  

private:
    CommandType type_;
};