#ifndef COMMAND_H
#define COMMAND_H

#include <string>

enum class CommandType {
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Attack,
    CastSpell,
    SaveGame,
    LoadGame,
    ShowInfo,
    Quit,
    None
};

class Command {
private:
    CommandType type_;
    std::string parameter_;  

public:
    Command() : type_(CommandType::None) {}
    Command(CommandType type) : type_(type) {}
    Command(CommandType type, const std::string& param) : type_(type), parameter_(param) {}

    CommandType getType() const { return type_; }
    const std::string& getParameter() const { return parameter_; }

    bool isValid() const { return type_ != CommandType::None; }
};

#endif
