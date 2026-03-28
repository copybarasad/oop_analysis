#ifndef COMMAND_H
#define COMMAND_H

#include <memory>
#include <string>
#include "GameController.h"
#include "Position.h"

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(GameController& controller) = 0;
    virtual std::string toString() const = 0;
};

class MoveCommand : public ICommand {
private:
    Position direction;
public:
    MoveCommand(const Position& dir) : direction(dir) {}
    void execute(GameController& controller) override;
    std::string toString() const override;
};

class AttackCommand : public ICommand {
private:
    Position direction;
public:
    AttackCommand(const Position& dir) : direction(dir) {}
    void execute(GameController& controller) override;
    std::string toString() const override;
};

class AreaAttackCommand : public ICommand {
public:
    void execute(GameController& controller) override;
    std::string toString() const override;
};

class SwitchCombatCommand : public ICommand {
public:
    void execute(GameController& controller) override;
    std::string toString() const override;
};

class UseSpellCommand : public ICommand {
private:
    int spellIndex;
    Position target;
public:
    UseSpellCommand(int index, const Position& targ) : spellIndex(index), target(targ) {}
    void execute(GameController& controller) override;
    std::string toString() const override;
};

class BuySpellCommand : public ICommand {
public:
    void execute(GameController& controller) override;
    std::string toString() const override;
};

class UpgradeSpellCommand : public ICommand {
private:
    int spellIndex;
public:
    UpgradeSpellCommand(int index) : spellIndex(index) {}
    void execute(GameController& controller) override;
    std::string toString() const override;
};

class ListSpellsCommand : public ICommand {
public:
    void execute(GameController& controller) override;
    std::string toString() const override;
};

class SaveGameCommand : public ICommand {
public:
    void execute(GameController& controller) override;
    std::string toString() const override;
};

class HelpCommand : public ICommand {
public:
    HelpCommand() = default;
    virtual ~HelpCommand() = default;
    
    void execute(GameController& controller) override;
    std::string toString() const override;
};

class QuitCommand : public ICommand {
public:
    void execute(GameController& controller) override;
    std::string toString() const override;
};
#endif