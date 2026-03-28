#include "Shop.h"
#include "Player.h"
#include "VisualEffects.h"
#include <iostream>

Shop::Shop(int x, int y) : m_x(x), m_y(y) {}

// === KEYBOARD METHODS ===

void Shop::tryBuySpell(Player* player) {
    if (player->getScore() >= SPELL_COST) {
        if (!player->getHand().isFull()) {
            player->addScore(-SPELL_COST);
            player->addRandomSpell();
        } else {
            VisualEffects::printColored("Hand is Full!", sf::Color::Red);
        }
    } else {
        VisualEffects::printColored("Need 300 Score!", sf::Color::Red);
    }
}

void Shop::tryUpgradeSpell(Player* player) {
    if (player->getScore() >= SPELL_UPGRADE_COST) {
        if (player->getHand().upgradeRandomSpell()) {
            player->addScore(-SPELL_UPGRADE_COST);
            VisualEffects::printColored("Random Spell Upgraded!", sf::Color::Magenta);
        } else {
            VisualEffects::printColored("Nothing to upgrade!", sf::Color::White);
        }
    } else {
        VisualEffects::printColored("Need 300 Score!", sf::Color::Red);
    }
}

// === MOUSE METHODS ===

void Shop::handleMouseClick(float mx, float my, Player* player, const sf::Vector2f&) {
    float startX = 100.0f;
    float startY = 150.0f;
    float boxW = 350.0f;
    float boxH = 50.0f;
    float gap = 20.0f;
    

    sf::FloatRect btn1({startX, startY}, {boxW, boxH});
    if (btn1.contains({mx, my})) {
        tryBuySpell(player);
        return;
    }
    
    sf::FloatRect btn2({startX, startY + (boxH+gap)}, {boxW, boxH});
    if (btn2.contains({mx, my})) {
        if (player->getScore() >= MELEE_UPGRADE_COST) {
            player->addScore(-MELEE_UPGRADE_COST);
            player->upgradeMeleeDamage();
            VisualEffects::printColored("Melee Damage Upgraded!", sf::Color::Green);
        } else VisualEffects::printColored("Need 400 Score!", sf::Color::Red);
        return;
    }

    // Button 3: Upgrade Ranged
    sf::FloatRect btn3({startX, startY + (boxH+gap)*2}, {boxW, boxH});
    if (btn3.contains({mx, my})) {
        if (player->getScore() >= RANGED_UPGRADE_COST) {
            player->addScore(-RANGED_UPGRADE_COST);
            player->upgradeRangedDamage();
            VisualEffects::printColored("Ranged Damage Upgraded!", sf::Color::Green);
        } else VisualEffects::printColored("Need 400 Score!", sf::Color::Red);
        return;
    }

    // Button 4: Upgrade HP
    sf::FloatRect btn4({startX, startY + (boxH+gap)*3}, {boxW, boxH});
    if (btn4.contains({mx, my})) {
        if (player->getScore() >= HP_UPGRADE_COST) {
            player->addScore(-HP_UPGRADE_COST);
            player->upgradeMaxHealth();
            player->heal(200);
            VisualEffects::printColored("Max HP Upgraded!", sf::Color::Green);
        } else VisualEffects::printColored("Need 500 Score!", sf::Color::Red);
        return;
    }
}