#ifndef SPELLLOGGINGSYSTEM_H
#define SPELLLOGGINGSYSTEM_H

#include <string>

class SpellLoggingSystem {
public:
    static void logSpellSearch(const std::string& spellName, const std::string& searchTarget);
    static void logAreaSelection(const std::string& spellName, int areaX, int areaY, int targetCount);
    static void logTrapPlacement(const std::string& spellName, int x, int y);
    static void logNoTargets(const std::string& spellName);
    static void logNoValidPositions(const std::string& spellName);
};

#endif // SPELLLOGGINGSYSTEM_H