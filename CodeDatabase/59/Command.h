#pragma once
class Game;

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(Game& game) = 0;
};

class MoveCommand : public Command {
    int dx, dy;
public:
    MoveCommand(int x, int y) : dx(x), dy(y) {}
    void execute(Game& game) override;
};

class ToggleWeaponCommand : public Command {
public:
    void execute(Game& game) override;
};

class ShopCommand : public Command {
public:
    void execute(Game& game) override;
};

class CastSpellCommand : public Command {
public:
    void execute(Game& game) override;
};

class SaveCommand : public Command {
public:
    void execute(Game& game) override;
};

class QuitCommand : public Command {
public:
    void execute(Game& game) override;
};

class NoOpCommand : public Command {
public:
    void execute(Game& game) override {}
};