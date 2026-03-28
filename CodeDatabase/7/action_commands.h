#pragma once
#include "command.h"
#include "Game.h"
#include <string>

class AttackCommand : public Command {
public:
    bool execute(Game& game, std::string& resultMessage) override {
        // Логика атаки теперь внутри Game.cpp, там же и лог
        resultMessage = game.playerAttack();
        return true;
    }
    std::string getType() const override { return "ATTACK"; }
};

class SwitchCombatCommand : public Command {
public:
    bool execute(Game& game, std::string& resultMessage) override {
        resultMessage = game.switchCombatMode();
        return true;
    }
    std::string getType() const override { return "SWITCH_COMBAT"; }
};