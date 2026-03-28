#pragma once
#include "GameControl.h"

class StatsRenderer{
public:
    StatsRenderer() = default;
    void printPlayerStats(GameControl& controller);
    void printGameInstructions() const;
    void PrintPlayersSpells(GameControl& controller);
    void printBattleInformation(GameControl& controller);
};


