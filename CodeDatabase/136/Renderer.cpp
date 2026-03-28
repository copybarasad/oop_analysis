#include "Renderer.h"

void Renderer::renderLevelInfo(int turnCount, int level) const {
    std::cout << "\nTurn: " << turnCount << " | Level: " << level << "\n";
}

void Renderer::renderPlayerStats(int health, int shield, int maxShield, int mana, int maxMana, int points, const std::string& mode) const {
    std::cout << "Health: " << health << " | Shield: " << shield << "/" << maxShield
              << " | Mana: " << mana << "/" << maxMana << " | Score: " << points
              << " | Mode: " << mode << "\n";
}

void Renderer::renderField(int width, int height, const std::vector<std::vector<char>>& fieldData) const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::cout << fieldData[y][x] << ' ';
        }
        std::cout << '\n';
    }
}

void Renderer::renderSymbolsLegend() const {
    std::cout << "Symbols: W-you, B-beast, T-building, X-tower, ^-trap, #-wall, ~-slow\n";
}

void Renderer::renderCombatInfo(bool isMeleeMode) const {
    if (isMeleeMode) {
        std::cout << "MELEE: Move into beasts to attack (adjacent cells)\n";
    } else {
        std::cout << "RANGED: Move cursor 2-3 cells away to ranged attack\n";
    }
}

void Renderer::renderControls() const {
    std::cout << "Controls: WASD-move/attack, 1-3-spells, P-save and menu, M-show spells, ";
    std::cout << "C-switch mode, B-buy random spell, I-show status, Q-quit to menu\n";
}