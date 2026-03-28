#ifndef COMMANDS_H
#define COMMANDS_H

#include "game.h"
#include <iostream>

class Command {
public:
    virtual void execute(game& g) = 0;
    virtual ~Command() = default;
    virtual bool passesTurn() const { return true; }
};

class MoveCommand : public Command {
    int dx, dy;
public:
    MoveCommand(int x, int y) : dx(x), dy(y) {}
    void execute(game& g) override {
        g.PlayerTurn(dx, dy);
    }
};

//need update
class CastSpellCommand : public Command {
public:
    void execute(game& g) override {
        g.castSpell();
    }
};

class SaveCommand : public Command {
public:
    void execute(game& g) override {
        try {
            g.saveGameToFile("game_save.txt");
            g.add_log("Game saved!\n");
        } 
        catch (const std::exception& e) {
            std::cout << "Save failed: " << e.what() << "\n";
        }
    }
    bool passesTurn() const override { return false; }
};

class SkipTurnCommand : public Command {
public:
    void execute(game& g) override {
        std::cout << "Turn skipped.\n";
    }
};

class LoadCommand : public Command {
public:
    void execute(game& g) override {
        try {
            g.loadGameDuringPlay("game_save.txt");
        } catch (const std::exception& e) {
            std::cout << "Load failed: " << e.what() << "\n";
        }
    }
    bool passesTurn() const override { return false; }
};


class ExitCommand : public Command {
public:
    void execute(game& g) override {
        std::cout << "Exiting...\n";
        g.quit();
    }
    bool passesTurn() const override { return false; }
};

#endif
