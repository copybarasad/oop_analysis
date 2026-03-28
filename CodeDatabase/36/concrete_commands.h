#ifndef CONCRETE_COMMANDS_H
#define CONCRETE_COMMANDS_H

#include "command.h"
#include <string>

// Команда перемещения
class MoveCommand : public Command {
    int dr_, dc_;
public:
    MoveCommand(int dr, int dc);
    bool Execute(World& world) override;
};

// Команда атаки
class AttackCommand : public Command {
public:
    bool Execute(World& world) override;
};

// Команда переключения режима (ближний/дальний бой)
class SwitchModeCommand : public Command {
public:
    bool Execute(World& world) override;
};

// Команда использования заклинания
class CastSpellCommand : public Command {
    int spell_idx_;
    std::pair<int, int> target_;
public:
    CastSpellCommand(int idx, std::pair<int, int> target);
    bool Execute(World& world) override;
};

// Команда сохранения
class SaveCommand : public Command {
    std::string filename_;
public:
    explicit SaveCommand(std::string filename);
    bool Execute(World& world) override;
};

// Команда загрузки
class LoadCommand : public Command {
    std::string filename_;
public:
    explicit LoadCommand(std::string filename);
    bool Execute(World& world) override;
};

// Команда выхода
class QuitCommand : public Command {
public:
    bool Execute(World& world) override;
    bool IsQuit() const override { return true; }
};

// Пустая команда (если ввод был некорректен)
class NoOpCommand : public Command {
public:
    bool Execute(World&) override { return false; }
};

#endif