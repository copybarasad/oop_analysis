#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class Player;

class Shop {
public:
    Shop(int x, int y);
    
    int getX() const { return m_x; }
    int getY() const { return m_y; }
    
    // Mouse Interaction
    void handleMouseClick(float mouseX, float mouseY, Player* player, const sf::Vector2f& winSize);
    
    // Keyboard Interaction (Restored)
    void tryBuySpell(Player* player);
    void tryUpgradeSpell(Player* player); // Upgrades a random spell in hand

private:
    int m_x, m_y;
    
    // Costs
    static constexpr int SPELL_COST = 300;
    static constexpr int SPELL_UPGRADE_COST = 300; // Cost for random spell upgrade
    static constexpr int MELEE_UPGRADE_COST = 400;
    static constexpr int RANGED_UPGRADE_COST = 400;
    static constexpr int HP_UPGRADE_COST = 500;
};
