#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>
#include <limits>
#include <memory>
#include "GameField.h"
#include "Player.h"
#include "HandSpell.h"
#include "Constants.h"
#include "InputInterface.h"

using namespace std;
using namespace GameConstants;

class Output {
public:
    virtual void displayField(GameField& field) = 0;
    virtual void displayPlayer(Player& player) = 0;
    virtual void displaySpells(Player& player) = 0;
    virtual void displayVictory(Game& game) = 0;
    virtual void displayDefeat(Game& game) = 0;
    virtual void displayFinishLevel(Game& game) = 0;
    virtual void displayUpgradeChoice(Game& game) = 0;
};

#endif