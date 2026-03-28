#ifndef COMMANDS_H
#define COMMANDS_H

#include "Command.h"
#include "Gamefield.h"
#include "Player.h"
#include "HandSpell.h"
#include "Constants.h"
#include <string>

using namespace GameConstants;
class Game;

class PlayeMoVeCommand : public Command {
private:
    std::string name;
    Player* player;
    GameField* field;
    char direction;

public:
    PlayeMoVeCommand(char inpDirection, GameField& inpField, Player& inpPlayer) : name("PlayeMoVeCommand"), 
                    player(&inpPlayer), field(&inpField), direction(inpDirection) {}
    
    void execute() override;
    void parse(const std::string& input) override;
    
    std::string getName() const override {
        return name;
    }
};


class PlayerAttackCommand : public Command {
private:
    std::string name;
    Player* player;
    GameField* field;
    char attackDirection;
    
public:
    PlayerAttackCommand(char dir, GameField& f, Player& p) : name("PlayerAttackCommand"), player(&p), field(&f), attackDirection(dir) {}
    
    void execute() override;
    void parse(const std::string& input) override;
    
    std::string getName() const override {
        return name;
    }
};


class SwitchAttackTypeCommand : public Command {
private:
    std::string name;
    Player* player;
    
public:
    SwitchAttackTypeCommand(Player& p) : name("SwitchAttackTypeCommand"), player(&p) {}
    
    void execute() override;
    void parse(const std::string& input) override;
    
    std::string getName() const override {
        return name;
    }
};


class UseSpellCommand : public Command {
private:
    std::string name;
    Player* player;
    GameField* field;
    SpellID spellChoice;
    int x;
    int y;
    
public:
    UseSpellCommand(SpellID choice, Player& p, GameField& f, int x, int y) 
        : name("UseSpellCommand"), spellChoice(choice), player(&p), field(&f), x(x), y(y) {}
    
    void execute() override;
    void parse(const std::string& input) override;
    
    std::string getName() const override {
        return name;
    }
};

class BuySpellCommand : public Command {
private:
    std::string name;
    Player* player;
    SpellID spellID;
    int count;
    
public:
    BuySpellCommand(Player& p, SpellID id, int cnt) 
        : name("BuySpellCommand"), player(&p), spellID(id), count(cnt) {}
    
    void execute() override;
    void parse(const std::string& input) override;
    
    std::string getName() const override {
        return name;
    }
};

class SaveGameCommand : public Command {
private:
    std::string name;
    Game& game;
    std::string saveName;
public:
    SaveGameCommand(Game& g, const std::string& sName) 
        : name("SaveGameCommand"), game(g), saveName(sName) {}
    
    void execute() override;
    void parse(const std::string& input) override;
    
    std::string getName() const override {
        return name;
    }
};

class LoadGameCommand : public Command {
private:
    std::string name;
    Game& game;
public:
    LoadGameCommand(Game& g) 
        : name("LoadGameCommand"), game(g) {}
    
    void execute() override;
    void parse(const std::string& input) override;
    
    std::string getName() const override {
        return name;
    }
};

class QuitCommand : public Command {
private:
    std::string name;
    Game& game;
public:
    QuitCommand(Game& g)
        : name("QuitCommand"), game(g) {}
    void execute() override;
    void parse(const std::string& input) override;
    std::string getName() const override {
        return name;
    }
};

#endif