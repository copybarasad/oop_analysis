#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>
#include <string>
#include <vector>

class Renderer {
public:
    void renderLevelInfo(int turnCount, int level) const;
    void renderPlayerStats(int health, int shield, int maxShield, int mana, int maxMana, int points, const std::string& mode) const;
    void renderField(int width, int height, const std::vector<std::vector<char>>& fieldData) const;
    void renderControls() const;
    void renderSymbolsLegend() const;
    void renderCombatInfo(bool isMeleeMode) const;
};

#endif