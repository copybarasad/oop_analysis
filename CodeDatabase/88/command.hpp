#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <memory>
#include <string>
#include "../managers/gamemanager.hpp"

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(Game& game) = 0;
    virtual std::string getName() const = 0;
};


class MoveCommand : public Command {
private:
    std::string direction;
    
public:
    explicit MoveCommand(const std::string& dir) : direction(dir) {}
    
    void execute(Game& game) override {
        game.processPlayerTurn(direction);
    }
    
    std::string getName() const override {
        return "move_" + direction;
    }
};

class SwitchWeaponCommand : public Command {
public:
    void execute(Game& game) override {
        game.processPlayerTurn("switch");
    }
    
    std::string getName() const override {
        return "switch_weapon";
    }
};

class CastSpellCommand : public Command {
private:
    int spellIndex;
    
public:
    explicit CastSpellCommand(int index) : spellIndex(index) {}
    
    void execute(Game& game) override {
        game.processPlayerTurn("cast " + std::to_string(spellIndex));
    }
    
    std::string getName() const override {
        return "cast_" + std::to_string(spellIndex);
    }
};

class EndGameCommand : public Command {
public:
    void execute(Game& game) override {
        game.saveGameState();
    }
    
    std::string getName() const override {
        return "end_game";
    }
};

class SaveGameCommand : public Command {
public:
    void execute(Game& game) override {
        game.saveGameState();
    }
    
    std::string getName() const override {
        return "save_game";
    }
};

class HelpCommand : public Command {
public:
    void execute(Game& game) override {
        
    }
    
    std::string getName() const override {
        return "help";
    }
};

#endif