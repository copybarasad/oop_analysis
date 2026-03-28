#pragma once
#include <string>

enum class CommandType {
    NONE,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    ATTACK,
    SWITCH_WEAPON,
    USE_SPELL,
    VIEW_SPELLS,
    SAVE_GAME,
    LOAD_GAME,
    QUIT,
    MENU_SELECT,
    YES,
    NO
};

class Command {
public:
    explicit Command(CommandType type) : type_(type) {}
    virtual ~Command() = default;

    CommandType GetType() const { return type_; }
    virtual bool IsValid() const { return type_ != CommandType::NONE; }

protected:
    CommandType type_;
};

class MoveCommand : public Command {
public:
    MoveCommand(CommandType type, int dx, int dy)
        : Command(type), dx_(dx), dy_(dy) {}

    int GetDX() const { return dx_; }
    int GetDY() const { return dy_; }

private:
    int dx_;
    int dy_;
};

class MenuCommand : public Command {
public:
    explicit MenuCommand(int choice)
        : Command(CommandType::MENU_SELECT), choice_(choice) {}

    int GetChoice() const { return choice_; }

private:
    int choice_;
};

class TextInputCommand : public Command {
public:
    TextInputCommand(CommandType type, const std::string& text)
        : Command(type), text_(text) {}

    const std::string& GetText() const { return text_; }

private:
    std::string text_;
};
