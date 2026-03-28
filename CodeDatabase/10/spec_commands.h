#pragma once
#include "command.h"

class MoveCommand : public Command {
    int dx, dy;
public:
    MoveCommand(int dx, int dy);
    bool execute(Game& game) override;
    std::string getName() const override;
};

class AttackCommand : public Command {
public:
    bool execute(Game& game) override;
    std::string getName() const override { return "ATTACK"; }
};

class ChangeCombatTypeCommand : public Command {
public:
    bool execute(Game& game) override;
    std::string getName() const override { return "CHANGE_TYPE"; }
};

class ShowStatusCommand : public Command {
public:
    bool execute(Game& game) override;
    std::string getName() const override { return "SHOW_STATUS"; }
};

class ShowSpellsCommand : public Command {
public:
    bool execute(Game& game) override;
    std::string getName() const override { return "SHOW_SPELLS"; }
};

class CastSpellCommand : public Command {
    int spellIndex, targetX, targetY;
public:
    CastSpellCommand(int spellIndex, int targetX, int targetY);
    bool execute(Game& game) override;
    std::string getName() const override { return "CAST_SPELL"; }
};

class BuySpellCommand : public Command {
public:
    bool execute(Game& game) override;
    std::string getName() const override { return "BUY_SPELL"; }
};

class SaveGameCommand : public Command {
public:
    bool execute(Game& game) override;
    std::string getName() const override { return "SAVE"; }
};

class LoadGameCommand : public Command {
public:
    bool execute(Game& game) override;
    std::string getName() const override { return "LOAD"; }
};

class QuitCommand : public Command {
public:
    bool execute(Game& game) override;
    std::string getName() const override { return "QUIT"; }
};

class ShowMenuCommand : public Command {
public:
    bool execute(Game& game) override;
    std::string getName() const override { return "SHOW_MENU"; }
};

class RestartCommand : public Command {
public:
    bool execute(Game& game) override;
    std::string getName() const override { return "RESTART"; }
};
