#include "GameRenderer.h"
#include "Player.h"
#include "GameField.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include "Spell.h"
#include <iostream>
#include <iomanip>
#include <cctype>
#include <algorithm>

void GameRenderer::renderStats(const Player& player, int turnCount,
    const std::vector<Enemy>& enemies,
    const std::vector<EnemyTower>& towers,
    const GameField& field) {

    std::cout << "\nTURN " << turnCount << " | HP: " << player.getHealth() << "/" << player.getMaxHealth()
        << " | MANA: " << player.getMana() << "/" << player.getMaxMana() << std::endl;

    int aliveEnemies = 0;
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) aliveEnemies++;
    }

    int aliveTowers = 0;
    for (const auto& tower : towers) {
        if (tower.isAlive()) aliveTowers++;
    }

    auto damageInfo = player.getDamageInfo();
    std::cout << "ATK: " << (player.getAttackType() == AttackType::Melee ? "MELEE" : "RANGED")
        << " (" << damageInfo.currentDamage << ") | POS: ("
        << player.getPositionX() << ", " << player.getPositionY() << ")";
    std::cout << " | ENEMIES: " << aliveEnemies << " | TOWERS: " << aliveTowers;
    std::cout << " | SCORE: " << player.getScore() << std::endl;
}

void GameRenderer::renderSpellInfo(const Player& player) {
    if (player.getHand().getSpellCount() == 0) {
        std::cout << "\nSPELLS: None available\n";
        return;
    }

    std::cout << "\nSPELLS (Mana: " << player.getMana() << "):\n";
    std::cout << "----------------------------------------\n";

    for (size_t i = 0; i < player.getHand().getSpellCount(); ++i) {
        auto* spell = player.getHand().getSpell(i);
        if (!spell) continue;

        bool canCast = player.getMana() >= spell->getManaCost();
        std::cout << (canCast ? "[+] " : "[-] ");

        std::cout << i + 1 << ". " << spell->getName();
        std::cout << " (" << spell->getManaCost() << " mana)";

        std::string desc = spell->getDescription();
        if (desc.length() > 40) {
            std::cout << " - " << desc.substr(0, 37) << "...";
        }
        else {
            std::cout << " - " << desc;
        }

        std::cout << std::endl;
    }
    std::cout << "----------------------------------------\n";
}

void GameRenderer::renderField(const GameField& field, const Player& player,
    const std::vector<Enemy>& enemies,
    const std::vector<EnemyTower>& towers) {

    char display[15][15];

    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            display[y][x] = '.';
        }
    }

    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            if (field.getCell(x, y) == '#') {
                display[y][x] = '#';
            }
        }
    }

    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            if (field.hasTrap(x, y)) {
                display[y][x] = 'X';
            }
        }
    }

    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            int x = enemy.getPositionX();
            int y = enemy.getPositionY();
            if (x >= 0 && x < field.getWidth() && y >= 0 && y < field.getHeight()) {
                display[y][x] = getHealthStatus('E', enemy.getHealth(), enemy.getMaxHealth())[0];
            }
        }
    }

    for (const auto& tower : towers) {
        if (tower.isAlive()) {
            int x = tower.getPositionX();
            int y = tower.getPositionY();
            if (x >= 0 && x < field.getWidth() && y >= 0 && y < field.getHeight()) {
                display[y][x] = getHealthStatus('T', tower.getHealth(), tower.getMaxHealth())[0];
            }
        }
    }

    int playerX = player.getPositionX();
    int playerY = player.getPositionY();
    if (playerX >= 0 && playerX < field.getWidth() && playerY >= 0 && playerY < field.getHeight()) {
        display[playerY][playerX] = 'P';
    }

    std::cout << "\n";
    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            std::cout << display[y][x] << ' ';
        }
        std::cout << std::endl;
    }

    std::cout << "\nP=Player, E/e/!=Enemy, T/t/+=Tower, X=Trap, #=Wall, .=Empty\n";
}

void GameRenderer::renderGameStart() {
    std::cout << "\n--- GAME STARTED ---\n";
    renderControls();
}

void GameRenderer::renderVictory(int turnCount, int score) {
    std::cout << "\n--- VICTORY ---\n";
    std::cout << "Turns: " << turnCount << " | Score: " << score << "\n";

    std::cout << "\nPerformance:\n";
    if (turnCount < 20) {
        std::cout << "Excellent! Very efficient!\n";
    }
    else if (turnCount < 40) {
        std::cout << "Great job!\n";
    }
    else if (turnCount < 60) {
        std::cout << "Good effort!\n";
    }
    else {
        std::cout << "Keep practicing!\n";
    }
}

void GameRenderer::renderGameOver(int turnCount, int score) {
    std::cout << "\n--- GAME OVER ---\n";
    std::cout << "Turns survived: " << turnCount << " | Final score: " << score << "\n";

    std::cout << "\nAssessment:\n";
    if (score > 1000) {
        std::cout << "Impressive! Master tactician!\n";
    }
    else if (score > 500) {
        std::cout << "Well played! Good strategy!\n";
    }
    else if (score > 100) {
        std::cout << "Not bad! Keep improving!\n";
    }
    else {
        std::cout << "Try again! Practice makes perfect!\n";
    }
}

void GameRenderer::renderInvalidCommand() {
    std::cout << "\nInvalid command!\n";
    renderControls();
}

void GameRenderer::renderControls() {
    std::cout << "\nWASD-move, E-attack, F-switch, C-spells, V-save, L-load, Q-quit, I-info\n";
}

std::string GameRenderer::getHealthStatus(char symbol, int health, int maxHealth) const {
    float percentage = static_cast<float>(health) / maxHealth;
    if (percentage > 0.66f) {
        return std::string(1, symbol);
    }
    else if (percentage > 0.33f) {
        return std::string(1, static_cast<char>(std::tolower(static_cast<unsigned char>(symbol))));
    }
    else {
        return "!";
    }
}