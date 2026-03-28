#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <memory>
#include "../enums/Enums.h"

class Game;

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(Game& game) = 0;
    virtual std::string getName() const = 0;
};

class MoveCommand : public Command {
    Direction direction;
public:
    MoveCommand(Direction dir);
    void execute(Game& game) override;
    std::string getName() const override;
};

class SwitchWeaponCommand : public Command {
public:
    void execute(Game& game) override;
    std::string getName() const override;
};

class RangedAttackCommand : public Command {
    Direction direction;
public:
    RangedAttackCommand(Direction dir);
    void execute(Game& game) override;
    std::string getName() const override;
};

class CastSpellCommand : public Command {
    int spellIndex;
    Direction direction;
    bool needsInput;  // команда требует ввода
public:
    CastSpellCommand();  // Без параметров - будет запрашивать ввод при выполнении
    CastSpellCommand(int idx, Direction dir);  // С параметрами
    void execute(Game& game) override;
    std::string getName() const override;
};

class BuySpellCommand : public Command {
public:
    void execute(Game& game) override;
    std::string getName() const override;
};

class SaveGameCommand : public Command {
public:
    void execute(Game& game) override;
    std::string getName() const override;
};

class LoadGameCommand : public Command {
public:
    void execute(Game& game) override;
    std::string getName() const override;
};

class QuitCommand : public Command {
public:
    void execute(Game& game) override;
    std::string getName() const override;
};

class NextLevelCommand : public Command {
public:
    void execute(Game& game) override;
    std::string getName() const override;
};

class RestartCommand : public Command {
public:
    void execute(Game& game) override;
    std::string getName() const override;
};

#endif