#ifndef COMMANDS_H
#define COMMANDS_H

#include "Game.h"

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(Game& game) = 0;
};

inline bool checkLevelUpState(Game& game) {
    if (game.getCurrentState() == GameState::kLevelUp) {
        game.notifyLog("Action blocked! You must choose a bonus first.");
        game.printLevelUpOptions(); 
        return true;
    }
    return false;
}

class MoveUpCommand : public Command {
public:
    void execute(Game& game) override {
        if (checkLevelUpState(game)) return;

        if (game.getCurrentState() == GameState::kCasting) {
            Point p = game.getCastingCursor();
            p.y = std::max(0, p.y - 1);
            game.setCastingCursor(p);
        } else {
            game.movePlayer(0, -1);
            game.endPlayerTurn();
        }
    }
};

class MoveDownCommand : public Command {
public:
    void execute(Game& game) override {
        if (checkLevelUpState(game)) return;

        if (game.getCurrentState() == GameState::kCasting) {
            Point p = game.getCastingCursor();
            p.y = std::min(game.getField().getHeight() - 1, p.y + 1);
            game.setCastingCursor(p);
        } else {
            game.movePlayer(0, 1);
            game.endPlayerTurn();
        }
    }
};

class MoveLeftCommand : public Command {
public:
    void execute(Game& game) override {
        if (checkLevelUpState(game)) return;

        if (game.getCurrentState() == GameState::kCasting) {
            Point p = game.getCastingCursor();
            p.x = std::max(0, p.x - 1);
            game.setCastingCursor(p);
        } else {
            game.movePlayer(-1, 0);
            game.endPlayerTurn();
        }
    }
};

class MoveRightCommand : public Command {
public:
    void execute(Game& game) override {
        if (checkLevelUpState(game)) return;

        if (game.getCurrentState() == GameState::kCasting) {
            Point p = game.getCastingCursor();
            p.x = std::min(game.getField().getWidth() - 1, p.x + 1);
            game.setCastingCursor(p);
        } else {
            game.movePlayer(1, 0);
            game.endPlayerTurn();
        }
    }
};

class AttackCommand : public Command { 
public:
    void execute(Game& game) override {
        if (checkLevelUpState(game)) return;

        if (game.getCurrentState() == GameState::kCasting) {
            game.castSelectedSpell();
        }
    }
};

class SwitchStanceCommand : public Command {
public:
    void execute(Game& game) override {
        if (checkLevelUpState(game)) return;

        if (game.getCurrentState() == GameState::kNormal) {
            game.getEntityManager().getPlayer()->switchStance();
            game.notifyLog("Player switched stance.");
            game.endPlayerTurn();
        }
    }
};

class EnterCastingCommand : public Command {
public:
    void execute(Game& game) override {
        if (checkLevelUpState(game)) return;

        if (game.getCurrentState() == GameState::kNormal) {
            game.enterCastingMode();
        }
    }
};

class ExitCastingCommand : public Command {
public:
    void execute(Game& game) override {
        if (game.getCurrentState() == GameState::kLevelUp) return;

        if (game.getCurrentState() == GameState::kCasting) {
            game.exitCastingMode();
        }
    }
};

class BuySpellCommand : public Command {
public:
    void execute(Game& game) override {
        if (checkLevelUpState(game)) return;

        if (game.getCurrentState() == GameState::kNormal) {
            game.buySpell();
        }
    }
};

class SaveCommand : public Command {
public:
    void execute(Game& game) override {
        game.saveGame("quicksave.txt");
    }
};

class LoadCommand : public Command {
public:
    void execute(Game& game) override {
        game.loadGame("quicksave.txt");
    }
};

class QuitCommand : public Command {
public:
    void execute(Game& game) override {
        game.quit();
    }
};

class SelectSpellCommand : public Command {
    int index_;
public:
    explicit SelectSpellCommand(int index) : index_(index) {}
    void execute(Game& game) override {
        if (game.getCurrentState() == GameState::kLevelUp) {
            game.selectLevelUpBonus(index_ + 1);
        }
        else if (game.getCurrentState() == GameState::kCasting) {
            game.selectSpell(index_);
        }
        else {
            game.notifyLog("Number keys are used for Casting or Level Up selection.");
        }
    }
};

class UnknownCommand : public Command {
private:
    char input_;
public:
    explicit UnknownCommand(char input) : input_(input) {}
    
    void execute(Game& game) override {
        game.notifyLog(std::string("Unknown command: '") + input_ + "'");
    }
};

class NullCommand : public Command {
public:
    void execute(Game& game) override {}
};

#endif