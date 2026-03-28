#ifndef GAMELB1_COMMAND_H
#define GAMELB1_COMMAND_H

#include "Game.h"
#include <iostream>

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(Game& game) = 0;
};


class MoveCommand : public Command {
    char direction_;
public:
    explicit MoveCommand(char dir) : direction_(dir) {}
    void execute(Game& game) override {
        if (game.getGameState() == GameState::Aiming) {
            game.processPlayerAim(direction_);
        } else {
            game.processPlayerMove(direction_);
        }
    }
};

class ActionCommand : public Command {
    char action_;
public:
    explicit ActionCommand(char act) : action_(act) {}
    void execute(Game& game) override {
        game.processPlayerAction(action_);
    }
};

class SelectSpellCommand : public Command {
    int index_;
public:
    explicit SelectSpellCommand(int index) : index_(index) {}
    void execute(Game& game) override {
        game.selectSpell(index_);
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

class NextLevelCommand : public Command {
public:
    void execute(Game& game) override {
        game.tryNextLevel();
    }
};

class RestartCommand : public Command {
public:
    void execute(Game& game) override {
        if (game.getGameState() == GameState::GameOver || game.getGameState() == GameState::Victory) {
            game.restartGame();
        }
    }
};

class UpgradeCommand : public Command {
    int choice_;
public:
    explicit UpgradeCommand(int choice) : choice_(choice) {}
    void execute(Game& game) override {
        game.upgradePlayer(choice_);
    }
};

class NoOpCommand : public Command {
public:
    void execute(Game& game) override {}
};

#endif //GAMELB1_COMMAND_H