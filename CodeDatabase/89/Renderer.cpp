#include "Renderer.h"
#include <iostream>

void ConsoleRenderer::render(const GameGrid& grid, const Player* player, 
                            size_t enemyCount, size_t towerCount) {
    grid.printGrid();
    if (player) {
        std::cout << "Player Health: " << player->getHealth()
                  << ", Score: " << player->getScore()
                  << ", Combat Mode: " << (player->getCombatMode() == CombatMode::Melee ? "Melee" : "Ranged")
                  << ", Spells: " << player->getSpellHand().size() << std::endl;
    }
    std::cout << "Active Enemies: " << enemyCount << ", Towers: " << towerCount << std::endl;
}

void ConsoleRenderer::renderMainMenu() {
    std::cout << "\n=== MAIN MENU ===" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "==================" << std::endl;
}

void ConsoleRenderer::renderGameOver() {
    std::cout << "\n💀 GAME OVER 💀" << std::endl;
    std::cout << "1. Restart Level" << std::endl;
    std::cout << "2. Exit" << std::endl;
    std::cout << "Choose option (1-2): ";
}

void ConsoleRenderer::renderUpgradeMenu(const Player& player) {
    std::cout << "\n=== LEVEL UP! Choose upgrade ===" << std::endl;
    std::cout << "1. +20 HP" << std::endl;
    std::cout << "2. +10 Melee Damage (" << player.getMeleeDamage() << " → " 
              << player.getMeleeDamage() + 10 << ")" << std::endl;
    std::cout << "3. +5 Ranged Damage (" << player.getRangedDamage() << " → " 
              << player.getRangedDamage() + 5 << ")" << std::endl;
    std::cout << "4. +1 Spell Slot" << std::endl;
    std::cout << "===============================" << std::endl;
}

void ConsoleRenderer::renderSpells(const Player& player) {
    std::cout << "\n--- Spells ---" << std::endl;
    for (size_t i = 0; i < player.getSpellHand().size(); ++i) {
        std::cout << "[" << i << "] " << player.getSpellHand().getSpellName(i) << std::endl;
    }
}