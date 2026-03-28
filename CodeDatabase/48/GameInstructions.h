#ifndef GAMEINSTRUCTIONS_H
#define GAMEINSTRUCTIONS_H

#include <string>

class GameInstructions {
public:
    static void displayInstructions();
    static std::string getMovementInstructions();
    static std::string getActionInstructions();
    static std::string getCombatInstructions();
    static std::string getSpellInstructions();
    static std::string getSymbolLegend();
    static std::string getObjective();
};

#endif