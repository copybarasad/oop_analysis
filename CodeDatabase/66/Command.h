#ifndef COMMAND_H
#define COMMAND_H

#include "Direction.h"
#include "Position.h"
#include <memory>
#include <string>

class Game;
class GameWorld;
class Player;

enum class CommandType {
    MOVE,
    ATTACK,
    SWITCH_MODE,
    SKIP_TURN,
    CAST_SPELL,
    BUY_SPELL,
    SAVE_GAME,
    LOAD_GAME,
    RESTART,
    QUIT,
    INVALID
};

class Command {
public:
    virtual ~Command() = default;
    virtual bool execute(Game& game) = 0;
    virtual CommandType getType() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::unique_ptr<Command> clone() const = 0;
};

class MoveCommand : public Command {
private:
    Direction direction;
public:
    explicit MoveCommand(Direction dir) : direction(dir) {}
    bool execute(Game& game) override;
    CommandType getType() const override { return CommandType::MOVE; }
    std::string getDescription() const override;
    Direction getDirection() const { return direction; }
    std::unique_ptr<Command> clone() const override {
        return std::make_unique<MoveCommand>(direction);
    }
};

class AttackCommand : public Command {
private:
    Direction direction;
public:
    explicit AttackCommand(Direction dir) : direction(dir) {}
    bool execute(Game& game) override;
    CommandType getType() const override { return CommandType::ATTACK; }
    std::string getDescription() const override;
    Direction getDirection() const { return direction; }
    std::unique_ptr<Command> clone() const override {
        return std::make_unique<AttackCommand>(direction);
    }
};

class SwitchModeCommand : public Command {
public:
    bool execute(Game& game) override;
    CommandType getType() const override { return CommandType::SWITCH_MODE; }
    std::string getDescription() const override { return "Switch combat mode"; }
    std::unique_ptr<Command> clone() const override {
        return std::make_unique<SwitchModeCommand>();
    }
};

class SkipTurnCommand : public Command {
public:
    bool execute(Game& game) override;
    CommandType getType() const override { return CommandType::SKIP_TURN; }
    std::string getDescription() const override { return "Skip turn"; }
    std::unique_ptr<Command> clone() const override {
        return std::make_unique<SkipTurnCommand>();
    }
};

class CastSpellCommand : public Command {
private:
    int spellIndex;
    Position targetPosition;
public:
    CastSpellCommand(int index, Position target) 
        : spellIndex(index), targetPosition(target) {}
    bool execute(Game& game) override;
    CommandType getType() const override { return CommandType::CAST_SPELL; }
    std::string getDescription() const override;
    int getSpellIndex() const { return spellIndex; }
    Position getTarget() const { return targetPosition; }
    std::unique_ptr<Command> clone() const override {
        return std::make_unique<CastSpellCommand>(spellIndex, targetPosition);
    }
};

class BuySpellCommand : public Command {
public:
    bool execute(Game& game) override;
    CommandType getType() const override { return CommandType::BUY_SPELL; }
    std::string getDescription() const override { return "Buy random spell"; }
    std::unique_ptr<Command> clone() const override {
        return std::make_unique<BuySpellCommand>();
    }
};

class SaveGameCommand : public Command {
public:
    bool execute(Game& game) override;
    CommandType getType() const override { return CommandType::SAVE_GAME; }
    std::string getDescription() const override { return "Save game"; }
    std::unique_ptr<Command> clone() const override {
        return std::make_unique<SaveGameCommand>();
    }
};

class RestartCommand : public Command {
public:
    bool execute(Game& game) override;
    CommandType getType() const override { return CommandType::RESTART; }
    std::string getDescription() const override { return "Restart game"; }
    std::unique_ptr<Command> clone() const override {
        return std::make_unique<RestartCommand>();
    }
};

class QuitCommand : public Command {
public:
    bool execute(Game& game) override;
    CommandType getType() const override { return CommandType::QUIT; }
    std::string getDescription() const override { return "Quit game"; }
    std::unique_ptr<Command> clone() const override {
        return std::make_unique<QuitCommand>();
    }
};

class InvalidCommand : public Command {
private:
    std::string errorMessage;
public:
    explicit InvalidCommand(const std::string& msg = "Invalid command") 
        : errorMessage(msg) {}
    bool execute(Game& game) override;
    CommandType getType() const override { return CommandType::INVALID; }
    std::string getDescription() const override { return errorMessage; }
    std::unique_ptr<Command> clone() const override {
        return std::make_unique<InvalidCommand>(errorMessage);
    }
};

#endif
