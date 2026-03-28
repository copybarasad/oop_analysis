#pragma once

#include <memory>

enum class CommandType { Move, ChangeAttackType, Attack, GiveASpell, Invalid };

class GameWorld;
class GameContext;

class Command {
public:
    virtual ~Command() = default;
    virtual CommandType getType() const = 0;
    virtual void execute(GameWorld& world, GameContext& context) = 0;
};

class MoveCommand : public Command {
public:
    MoveCommand(int dx, int dy) : dx_(dx), dy_(dy) {}
    CommandType getType() const override { return CommandType::Move; }
    int getDx() const { return dx_; }
    int getDy() const { return dy_; }

    void execute(GameWorld& world, GameContext& context) override;

private:
    int dx_, dy_;
};

class ChangeAttackTypeCommand : public Command {
public:
    ChangeAttackTypeCommand(int newType) : newType_(newType) {}
    CommandType getType() const override { return CommandType::ChangeAttackType; }
    int getNewType() const { return newType_; }

    void execute(GameWorld& world, GameContext& context) override;

private:
    int newType_;
};

class AttackCommand : public Command {
public:
    CommandType getType() const override { return CommandType::Attack; }
    void execute(GameWorld& world, GameContext& context) override;
};

class GiveASpellCommand : public Command {
public:
    CommandType getType() const override { return CommandType::GiveASpell; }
    void execute(GameWorld& world, GameContext& context) override;
};

class InvalidCommand : public Command {
public:
    CommandType getType() const override { return CommandType::Invalid; }
    void execute(GameWorld& world, GameContext& context) override {}
};
