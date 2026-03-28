#include "SpellLoggingSystem.h"
#include <iostream>

void SpellLoggingSystem::logSpellSearch(const std::string& spellName, const std::string& searchTarget) {
    std::cout << "Casting " << spellName << " - searching for " << searchTarget << "..." << std::endl;
}

void SpellLoggingSystem::logAreaSelection(const std::string& spellName, int areaX, int areaY, int targetCount) {
    std::cout << spellName << " exploded at area (" << areaX << "," << areaY << ") to ("
        << areaX + 1 << "," << areaY + 1 << ")";

    if (targetCount > 0) {
        std::cout << " and hit " << targetCount << " targets!" << std::endl;
    }
    else {
        std::cout << " but hit no targets." << std::endl;
    }
}

void SpellLoggingSystem::logTrapPlacement(const std::string& spellName, int x, int y) {
    std::cout << spellName << " placed at (" << x << "," << y << ")!" << std::endl;
}

void SpellLoggingSystem::logNoTargets(const std::string& spellName) {
    std::cout << "No targets in range for " << spellName << "!" << std::endl;
}

void SpellLoggingSystem::logNoValidPositions(const std::string& spellName) {
    std::cout << "No valid positions for " << spellName << " in range!" << std::endl;
}