#include "renderer.h"
#include "game_field.h"
#include "player.h"
#include "enemy.h"
#include "enemy_building.h"
#include "enemy_tower.h"
#include "spell_manager.h"
#include <iostream>

void Renderer::displayField(const GameField& field, const Player& player,
                            const std::vector<Enemy>& enemies,
                            const std::vector<EnemyBuilding>& buildings,
                            const std::vector<EnemyTower>& towers,
                            const std::vector<Ally>& allies,
                            const std::vector<Trap>& traps) const {
    std::cout << "\nGame Field:\n";
    std::cout << "Legend: P=Player, E=Enemy, B=Building, T=Tower, A=Ally, X=Trap, #=Impassable, S=Slowing\n\n";

    auto playerPos = player.getPosition();

    for (size_t y = 0; y < field.getHeight(); ++y) {
        for (size_t x = 0; x < field.getWidth(); ++x) {
            if (playerPos.first == (int)x && playerPos.second == (int)y) {
                std::cout << "P ";
                continue;
            }
            bool printed = false;
            for (const auto& tower : towers) {
                if (tower.getPosition().first == (int)x && tower.getPosition().second == (int)y) {
                    std::cout << "T "; printed = true; break;
                }
            }
            if (printed) continue;
            for (const auto& building : buildings) {
                if (building.getPosition().first == (int)x && building.getPosition().second == (int)y) {
                    std::cout << "B "; printed = true; break;
                }
            }
            if (printed) continue;
            for (const auto& enemy : enemies) {
                if (enemy.isAlive() && enemy.getPosition().first == (int)x && enemy.getPosition().second == (int)y) {
                    std::cout << "E "; printed = true; break;
                }
            }
            if (printed) continue;
            for (const auto& ally : allies) {
                if (ally.getPosition().first == (int)x && ally.getPosition().second == (int)y) {
                    std::cout << "A "; printed = true; break;
                }
            }
            if (printed) continue;
            for (const auto& trap : traps) {
                if (trap.x == (int)x && trap.y == (int)y) {
                    std::cout << "X "; printed = true; break;
                }
            }
            if (printed) continue;

            if (field.isSlowing((int)x, (int)y)) {
                std::cout << "S ";
            } else if (!field.isPassable((int)x, (int)y)) {
                std::cout << "# ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void Renderer::displayPlayerStatus(const Player& player) const {
    std::cout << "Player - HP: " << player.getHealth() << "/" << player.getMaxHealth()
              << " | Mode: " << (player.getCombatMode() == CombatMode::MELEE ? "MELEE" : "RANGED")
              << " | Melee DMG: " << player.getMeleeDamage()
              << " | Ranged DMG: " << player.getRangedDamage()
              << " | Score: " << player.getScore() << '\n';
}

void Renderer::displaySpells(const SpellManager& spellManager) const {
    const auto& spells = spellManager.getHand().getSpells();
    std::cout << "Spells in hand (" << spells.size() << "/" << spellManager.getHand().getMaxSize() << "): ";
    for (size_t i = 0; i < spells.size(); ++i) {
        std::cout << "[" << i << "]" << spells[i]->getName();
        if (i < spells.size() - 1) std::cout << ", ";
    }
    if (spellManager.getEnhancementLevel() > 0) {
        std::cout << " | Enhancements: " << spellManager.getEnhancementLevel();
    }
    std::cout << "\n";
}