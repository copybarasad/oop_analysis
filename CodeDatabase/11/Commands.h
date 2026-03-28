#ifndef COMMANDS_H
#define COMMANDS_H

class GameController;

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(GameController& game) = 0;
};

class MoveCommand : public ICommand {
    int dx, dy;
public:
    MoveCommand(int _dx, int _dy);
    void execute(GameController& game) override;
};

class SwitchModeCommand : public ICommand {
public:
    void execute(GameController& game) override;
};

class CastSpellCommand : public ICommand {
public:
    void execute(GameController& game) override;
};

class BuySpellCommand : public ICommand {
public:
    void execute(GameController& game) override;
};

class SaveCommand : public ICommand {
public:
    void execute(GameController& game) override;
};

class LoadCommand : public ICommand {
public:
    void execute(GameController& game) override;
};

class ExitCommand : public ICommand {
public:
    void execute(GameController& game) override;
};

#endif