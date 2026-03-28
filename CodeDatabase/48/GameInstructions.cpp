#include "lib/GameInstructions.h"
#include <iostream>

void GameInstructions::displayInstructions() {
    std::cout << "\n=== GAME INSTRUCTIONS ===\n";
    std::cout << getMovementInstructions();
    std::cout << getActionInstructions();
    std::cout << getCombatInstructions();
    std::cout << getSpellInstructions();
    std::cout << getSymbolLegend();
    std::cout << getObjective();
    std::cout << "=======================\n";
}

std::string GameInstructions::getMovementInstructions() {
    return "MOVEMENT:\n"
           "  W - Move Up\n"
           "  S - Move Down\n"
           "  A - Move Left\n"
           "  D - Move Right\n";
}

std::string GameInstructions::getActionInstructions() {
    return "ACTIONS:\n"
           "  H - Heal (+20 HP)\n"
           "  C - Cast Spell\n"
           "  K - Quick Save\n"
           "  L - Load Game\n"
           "  T - Test Spells (Get all spells)\n"
           "  X - Exit Game\n";
}

std::string GameInstructions::getCombatInstructions() {
    return "COMBAT:\n"
           "  - Automatically attack when adjacent to enemy\n"
           "  - Enemies move toward you each turn\n"
           "  - Use spells for special effects\n";
}

std::string GameInstructions::getSpellInstructions() {
    return "SPELLS:\n"
           "  - Fire Bolt: Direct damage to single enemy\n"
           "  - Fireball: Area damage in 2x2 radius\n"
           "  - Bear Trap: Place trap that damages enemies\n"
           "  - Gain new spells by earning score (or use T to test)\n";
}

std::string GameInstructions::getSymbolLegend() {
    return "SYMBOLS:\n"
           "  P - Player (You)\n"
           "  E - Enemy\n"
           "  T - Enemy Tower\n"
           "  # - Wall (Cannot pass)\n"
           "  . - Empty space\n";
}

std::string GameInstructions::getObjective() {
    return "OBJECTIVE:\n"
           "  Defeat all enemies and towers to win!\n";
}