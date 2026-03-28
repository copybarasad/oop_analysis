#ifndef COMMAND_H
#define COMMAND_H

enum class CommandType {
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    ChangeStyle,
    Skip,
    Quit,
    Save,
    AttackCell,
    CastSpell,
    Invalid
};

struct Command {
    CommandType type = CommandType::Invalid;

    int x = 0;
    int y = 0;
    
    int spell_index = 0;
};

class ICommandInput {
public:
    virtual Command getCommand() = 0;
    virtual ~ICommandInput() = default;
};

#endif