#ifndef COMMAND_H
#define COMMAND_H

#include "GameEnums.h"
#include <string>
#include <memory>

class GameController;

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(GameController& controller) const = 0;
    virtual std::string getDescription() const = 0;
};

class MoveCommand : public Command {
private:
    Direction direction;
    
public:
    explicit MoveCommand(Direction dir);
    void execute(GameController& controller) const override;
    std::string getDescription() const override;
    Direction getDirection() const { return direction; }
};

class AttackCommand : public Command {
private:
    Direction direction;
    
public:
    explicit AttackCommand(Direction dir);
    void execute(GameController& controller) const override;
    std::string getDescription() const override;
    Direction getDirection() const { return direction; }
};

class SwitchModeCommand : public Command {
public:
    void execute(GameController& controller) const override;
    std::string getDescription() const override;
};

class CastSpellCommand : public Command {
private:
    int spellIndex;
    int targetX;
    int targetY;
    
public:
    CastSpellCommand(int index, int x, int y);
    void execute(GameController& controller) const override;
    std::string getDescription() const override;
    
    int getSpellIndex() const { return spellIndex; }
    std::pair<int, int> getTarget() const { return {targetX, targetY}; }
};

class PurchaseSpellCommand : public Command {
public:
    void execute(GameController& controller) const override;
    std::string getDescription() const override;
};

class SaveGameCommand : public Command {
public:
    void execute(GameController& controller) const override;
    std::string getDescription() const override;
};

class QuitCommand : public Command {
public:
    void execute(GameController& controller) const override;
    std::string getDescription() const override;
};

#endif
