#include "console_renderer.h"
#include "game.h"
#include "player.h"
#include "game_field.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

void ConsoleRenderer::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ConsoleRenderer::render(const Game& game) {
    clearScreen();
    
    // Информация об игроке
    renderPlayerInfo(game);
    std::cout << "----------------------------------------\n";
    
    // Заклинания
    renderSpells(game);
    std::cout << "----------------------------------------\n";
    
    // Поле
    renderField(game);
    std::cout << "----------------------------------------\n";
    
    std::cout << "Press 'H' for controls, 'Q' to quit\n";
}

void ConsoleRenderer::renderPlayerInfo(const Game& game) {
    const auto player = game.getPlayer();
    if (!player) return;
    
    std::cout << "Player: HP " << player->getHealth() 
              << "/" << player->getMaxHealth()
              << " | Mana: " << player->getMana()
              << "/" << player->getMaxMana()
              << " | Mode: " << (player->getCombatMode() ? "MELEE" : "RANGED")
              << " | Score: " << player->getScore()
              << " | Level: " << game.getCurrentLevel() << std::endl;
}

void ConsoleRenderer::renderSpells(const Game& game) {
    const auto player = game.getPlayer();
    if (!player) return;
    
    const auto& hand = player->getSpellHand();
    std::cout << "Spells: ";
    
    for (size_t i = 0; i < hand.getSize(); ++i) {
        auto spell = hand.getSpell(i);
        if (spell) {
            std::cout << "[" << (i + 1) << "]" << spell->getName() 
                     << "(" << spell->getManaCost() << ") ";
        }
    }
    std::cout << std::endl;
}

void ConsoleRenderer::renderField(const Game& game) {
    const auto field = game.getField();
    if (!field) return;
    
    for (int y = 0; y < static_cast<int>(field->getHeight()); ++y) {
        for (int x = 0; x < static_cast<int>(field->getWidth()); ++x) {
            std::cout << field->getCellSymbol(x, y) << " ";
        }
        std::cout << std::endl;
    }
}

void ConsoleRenderer::renderMenu() {
    clearScreen();
    std::cout << "=== DUNGEON ADVENTURE ===\n"
              << "1. New Game\n"
              << "2. Load Game\n"
              << "3. Quit\n"
              << "=========================\n"
              << "Select option (1-3): ";
}

void ConsoleRenderer::renderGameOver() {
    clearScreen();
    std::cout << "=== GAME OVER ===\n"
              << "Press 'R' to restart\n"
              << "Press 'Q' to quit\n"
              << "==================\n";
}

void ConsoleRenderer::renderControls() {
    clearScreen();
    std::cout << "=== CONTROLS ===\n"
              << "WASD - Move\n"
              << "M - Switch combat mode\n"
              << "F - Ranged attack\n"
              << "1-5 - Cast spells\n"
              << "Z - Save game\n"
              << "X - Load game\n"
              << "R - Restart\n"
              << "Q - Quit to menu\n"
              << "H - Show controls\n"
              << "===============\n"
              << "Press any key to continue...\n";
}