#pragma once
#include "GameLogic.h"

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(GameLogic& game) = 0;
};

class MoveCommand : public Command {
    int dx, dy;
public:
    MoveCommand(int x, int y) : dx(x), dy(y) {}
    void execute(GameLogic& game) override {
        game.movePlayer({dx, dy});
    }
};

class UseSpellCommand : public Command {
    int targetX, targetY;
    int spellIndex;
public:
    UseSpellCommand(int x, int y, int index = -1)
        : targetX(x), targetY(y), spellIndex(index) {}

    void execute(GameLogic& game) override {
         game.castSpell(targetX, targetY, spellIndex);
    }
};

class SelectUpgradeCommand : public Command {
    int choice;
public:
    SelectUpgradeCommand(int c) : choice(c) {}
    void execute(GameLogic& game) override {
        if (game.getState() == GameState::LevelUpMenu) {
            if (choice == 1) game.upgradeHealth();
            if (choice == 2) game.upgradeDamage();
            if (choice == 3) game.upgradeSpell();
        }
    }
};

class MenuCommand : public Command {
    std::string action;
public:
    MenuCommand(std::string act) : action(act) {}
    void execute(GameLogic& game) override {
        if (action == "NEW") game.startNewGame();
        if (action == "LOAD") game.loadGame("savegame.dat");
        if (action == "EXIT") game.exitGame();
        if (action == "CONTINUE") game.continueGame();
        if (action == "MENU") game.returnToMenu();
    }
};

class GeneralActionCommand : public Command {
    std::string action;
public:
    GeneralActionCommand(std::string act) : action(act) {}
    void execute(GameLogic& game) override {
        if (action == "SHOP") game.buySpell();
        if (action == "SWITCH_MODE") game.toggleCombatMode();
        if (action == "SAVE") game.saveGame("savegame.dat");
    }
};