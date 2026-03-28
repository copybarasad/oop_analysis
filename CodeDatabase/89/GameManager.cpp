#include "GameManager.h"
#include "Spell.h"
#include "DirectDamageSpell.h"
#include <iostream>

GameManager::GameManager(int width, int height, int impassableCellsCount)
    : grid(width, height, impassableCellsCount),
      movementSystem(grid),
      combatSystem(grid),
      spawnSystem(grid) {}

void GameManager::placePlayer(std::unique_ptr<Player> player) {
    this->player = std::move(player);
    if (this->player) {
        int x = this->player->getPosition().first;
        int y = this->player->getPosition().second;
        grid.updateCell(x, y, CellType::Player);
    }
}

void GameManager::addEnemy(std::unique_ptr<Enemy> enemy) {
    if (enemy) {
        int x = enemy->getPosition().first;
        int y = enemy->getPosition().second;
        grid.updateCell(x, y, CellType::Enemy);
        enemies.push_back(std::move(enemy));
    }
}

void GameManager::addTower(std::unique_ptr<EnemyTower> tower) {
    if (!tower) return;
    int x = tower->getPosition().first;
    int y = tower->getPosition().second;
    grid.updateCell(x, y, CellType::Tower);
    towers.push_back(std::move(tower));
}

bool GameManager::castPlayerSpell(int spellIndex, Direction direction) {
    if (!player || !player->isAlive()) return false;
    auto& hand = player->getSpellHand();
    auto spell = hand.useSpell(spellIndex);
    if (!spell) {
        std::cout << "Invalid spell index." << std::endl;
        return false;
    }

    bool success = spell->cast(*player, direction, enemies, towers, grid);
    if (success) {
        return true;
    } else {
        hand.addSpell(std::move(spell));
        return false;
    }
}

bool GameManager::movePlayer(Direction direction) {
    if (!player || !player->isAlive()) return false;
    return movementSystem.movePlayer(*player, direction, enemies);
}

void GameManager::moveEnemies() {
    if (!player || !player->isAlive()) return;
    movementSystem.moveEnemies(enemies, *player);
}

void GameManager::playerAttack(Direction direction) {
    if (!player || !player->isAlive()) return;
    combatSystem.playerAttack(*player, direction, enemies);
}

void GameManager::processTowerAttacks() {
    if (!player || !player->isAlive()) return;

    int px = player->getPosition().first;
    int py = player->getPosition().second;

    for (auto& tower : towers) {
        if (!tower || !tower->isAlive()) continue;
        if (!tower->canAttackThisTurn()) continue;

        auto pos = tower->getPosition();
        int tx = pos.first;
        int ty = pos.second;
        int range = tower->getAttackRange();

        int dx = std::abs(px - tx);
        int dy = std::abs(py - ty);

        if (dx <= range && dy <= range) {
            std::cout << "Enemy tower at (" << tx << "," << ty << ") casts Direct Damage spell on player!" << std::endl;
            player->takeDamage(tower->getDamage());
            tower->markAttackUsed();
        }
    }
}

void GameManager::updateTowers() {
    for (auto& tower : towers) {
        if (tower) {
            tower->update();
        }
    }
}

void GameManager::printField() const {
    grid.printGrid();
    if (player) {
        std::cout << "Player Health: " << player->getHealth()
                  << ", Score: " << player->getScore()
                  << ", Combat Mode: " << (player->getCombatMode() == CombatMode::Melee ? "Melee" : "Ranged")
                  << ", Spells: " << player->getSpellHand().size() << std::endl;
    }
    std::cout << "Active Enemies: " << enemies.size() << ", Towers: " << towers.size() << std::endl;
}

Player* GameManager::getPlayer() const {
    return player.get();
}

size_t GameManager::getEnemyCount() const {
    return enemies.size();
}

bool GameManager::isGameOver() const {
    return !player || !player->isAlive();
}

bool GameManager::isGameWon() const {
    return getEnemyCount() == 0;
}

std::unique_ptr<Player> GameManager::getPlayerPtr() {
    return std::move(player);
}

void GameManager::clearEnemiesAndTowers() {
    enemies.clear();
    towers.clear();
    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            CellType type = grid.getCell(j, i).getType();
            if (type == CellType::Enemy || type == CellType::Tower) {
                grid.updateCell(j, i, CellType::Empty);
            }
        }
    }
}