#ifndef OUTPUTINTERMINAL_H
#define OUTPUTINTERMINAL_H

#include "Output.h"


class Game;
class GameField;
class Player;

class OutputTerminal : public Output{
public:
    OutputTerminal() = default;

    void displayField(GameField& field) override;
    void displayPlayer(Player& player) override;
    void displaySpells(Player& player) override;
    void displayVictory(Game& game) override;
    void displayDefeat(Game& game) override;
    void displayFinishLevel(Game& game) override;
    void displayUpgradeChoice(Game& game) override;
};

#endif