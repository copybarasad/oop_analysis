#ifndef COMMANDS_H
#define COMMANDS_H

class Game; 

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(Game& game) = 0;
};

class MoveCommand : public Command {
    int dx, dy;
public:
    MoveCommand(int x, int y);
    void execute(Game& game) override;
};

class AttackCommand : public Command {
public:
    void execute(Game& game) override;
};

class SwitchWeaponCommand : public Command {
public:
    void execute(Game& game) override;
};

class CastSpellCommand : public Command {
    int spellIndex;
    int targetX, targetY;
public:
    CastSpellCommand(int idx, int tx, int ty);
    void execute(Game& game) override;
};

class SaveCommand : public Command {
public:
    void execute(Game& game) override;
};

class LoadCommand : public Command {
public:
    void execute(Game& game) override;
};

class ExitCommand : public Command {
public:
    void execute(Game& game) override;
};

class NullCommand : public Command {
public:
    void execute(Game& game) override;
};

#endif