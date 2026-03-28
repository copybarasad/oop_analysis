#include "SpellDirectDamage.hpp"
#include <cmath>
#include <iostream>
#include "SpellEnhancement.hpp"
SpellDirectDamage::SpellDirectDamage() {
    damage = 5;
    standart_range = 4;
    range = 4;
    mana_cost= 5;
}

int SpellDirectDamage::get_damage() const {
    return damage;
}

int SpellDirectDamage::get_range() const {
    return range;
}

int SpellDirectDamage::get_standart_range() const {
    return standart_range;
}

int SpellDirectDamage::get_mana_cost() const {
    return mana_cost;
}

bool SpellDirectDamage::requires_target() const {
    return true;
}

void SpellDirectDamage::set_range(int new_range) {
    range = new_range;
}

std::string SpellDirectDamage::get_name() const {
    return "Direct Damage";
}

bool SpellDirectDamage::cast(Player& player, Board& board, std::vector<Enemy>& enemies,
                      std::vector<Ally>& allies,
                      EnemyAttackTower& attack_tower, EnemyTower& tower,
                      int targetX, int targetY) {

    std::cout << "=== DIRECT DAMAGE DEBUG ===" << std::endl;
    std::cout << "Target: (" << targetX << ", " << targetY << ")" << std::endl;

    int current_range = range;
    if (SpellEnhancement::has_enhancement()) {
        current_range = SpellEnhancement::apply_direct_damage_enhancement(range);
    }

    int playerX, playerY;
    player.get_coords(playerX, playerY);
    std::cout << "Player position: (" << playerX << ", " << playerY << ")" << std::endl;

    int distance = std::abs(targetX - playerX) + std::abs(targetY - playerY);
    std::cout << "Distance to target: " << distance << ", Max range: " << current_range << std::endl;

    if (distance > current_range) {
        std::cout << "FAIL: Target is too far away" << std::endl;
        std::cout << "=== DIRECT DAMAGE END ===" << std::endl;
        return false;
    }

    // ПРОВЕРЯЕМ ВРАГОВ - ДОБАВЛЯЕМ ПОДРОБНУЮ ОТЛАДКУ
    bool enemy_found = false;
    for (auto& enemy : enemies) {
        int enemyX, enemyY;
        enemy.get_coords(enemyX, enemyY);
        std::cout << "Checking enemy at (" << enemyX << ", " << enemyY << ") vs target (" << targetX << ", " << targetY << ")" << std::endl;

        if (enemyX == targetX && enemyY == targetY && !enemy.is_dead()) {
            std::cout << "SUCCESS: Hitting enemy at (" << targetX << ", " << targetY << ") for " << damage << " damage" << std::endl;
            enemy.take_damage(damage);
            std::cout << "=== DIRECT DAMAGE END ===" << std::endl;
            return true;
        }
    }

    // ПРОВЕРЯЕМ АТАКУЮЩУЮ БАШНЮ - БОЛЕЕ ТОЧНО
    int attackTowerX, attackTowerY;
    attack_tower.get_coords(attackTowerX, attackTowerY);
    std::cout << "Attack tower at (" << attackTowerX << ", " << attackTowerY << ") vs target (" << targetX << ", " << targetY << ")" << std::endl;
    std::cout << "Attack tower dead: " << attack_tower.is_death() << std::endl;

    if (attackTowerX == targetX && attackTowerY == targetY && !attack_tower.is_death()) {
        std::cout << "SUCCESS: Hitting attack tower at (" << targetX << ", " << targetY << ") for " << damage << " damage" << std::endl;
        attack_tower.take_damage(damage);
        std::cout << "=== DIRECT DAMAGE END ===" << std::endl;
        return true;
    }

    // ПРОВЕРЯЕМ БАШНЮ-СПАВНЕР
    int spawnTowerX, spawnTowerY;
    tower.get_coords(spawnTowerX, spawnTowerY);
    std::cout << "Spawn tower at (" << spawnTowerX << ", " << spawnTowerY << ") vs target (" << targetX << ", " << targetY << ")" << std::endl;

    if (spawnTowerX == targetX && spawnTowerY == targetY) {
        std::cout << "SUCCESS: Hitting spawn tower at (" << targetX << ", " << targetY << ") for " << damage << " damage" << std::endl;
        tower.take_damage(damage);
        std::cout << "=== DIRECT DAMAGE END ===" << std::endl;
        return true;
    }

    std::cout << "FAIL: No valid target found at (" << targetX << ", " << targetY << ")" << std::endl;
    std::cout << "=== DIRECT DAMAGE END ===" << std::endl;
    return false;
}