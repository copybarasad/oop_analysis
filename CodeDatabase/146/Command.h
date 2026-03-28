#pragma once
#include <string>

class Game;

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(Game& game) = 0;
};

class MoveCommand : public Command {
private:
    std::string direction;
public:
    MoveCommand(std::string dir);
    void execute(Game& game) override;
};

class SpellCommand : public Command {
public:
    void execute(Game& game) override;
};

class SaveCommand : public Command {
public:
    void execute(Game& game) override;
};

class ExitCommand : public Command {
public:
    void execute(Game& game) override;
};

class EmptyCommand : public Command {
public:
    void execute(Game& game) override;
};
