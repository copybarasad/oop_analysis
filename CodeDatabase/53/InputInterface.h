#ifndef INPUTINTERFACE_H
#define INPUTINTERFACE_H

#include <iostream>
#include <limits>
#include <memory>
#include "GameField.h"
#include "Player.h"
#include "HandSpell.h"
#include "Constants.h"
#include "Command.h"

using namespace std;
using namespace GameConstants;
class Game;

class InputInterface {
public:
    virtual bool validateCoordinates(int x, int y, const GameField& field) = 0;
    virtual bool validateDirection(char direction) = 0;
    virtual void clearInputBuffer() = 0;
    virtual std::unique_ptr<Command> processSpellInput(SpellID spellID, Player& player, GameField& field) = 0;
    virtual std::unique_ptr<Command> processShopInput(Player& player) = 0;

    virtual int getLevelChoice() = 0;
    virtual int getMainMenuChoice() = 0; 
    virtual std::string getSaveName() = 0;
    virtual std::string getPlayerName() = 0;
    virtual int getLoadChoice(const std::vector<std::string>& saves) = 0;
    virtual char getLevelTransitionChoice() = 0;
    virtual int getUpgradeChoice() = 0;

    virtual char getPlayerInput() = 0;
    virtual std::unique_ptr<Command> processInput(char input, Player& player, GameField& field, Game& game) = 0;
};

#endif