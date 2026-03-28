#include "SpellSplash.hpp"
#include <iostream>
#include <cmath>
#include "SpellEnhancement.hpp"
SpellSplash::SpellSplash() {
    range = 4;
    mana_cost = 10;
    damage = 5;
    area_size=1;
}

int SpellSplash::get_damage() const {
    return damage;
}

int SpellSplash::get_range() const {
    return range;
}

int SpellSplash::get_mana_cost() const {
    return mana_cost;
}

bool SpellSplash::requires_target() const {
    return true;
}

std::string SpellSplash::get_name() const {
    return "Splash Damage";
}

bool SpellSplash::cast(Player& player, Board& board, std::vector<Enemy>& enemies,
                      std::vector<Ally>& allies,
                      EnemyAttackTower& attack_tower, EnemyTower& tower,
                      int targetX, int targetY) {

    std::cout << "=== SPLASH SPELL DEBUG ===" << std::endl;
    std::cout << "Target center: (" << targetX << ", " << targetY << ")" << std::endl;

    int current_area = area_size;
    if (SpellEnhancement::has_enhancement()) {
        current_area = SpellEnhancement::apply_splash_enhancement(area_size);
    }

    int playerX, playerY;
    player.get_coords(playerX, playerY);
    int distance = std::abs(targetX - playerX) + std::abs(targetY - playerY);
    if (distance > range) {
        std::cout << "Target area is too far away. Distance: " << distance << ", Range: " << range << std::endl;
        std::cout << "=== SPLASH SPELL END ===" << std::endl;
        return false;  // Только если слишком далеко
    }

    int length = board.get_length();
    int width = board.get_width();
    bool hit_anything = false;

    // Проверяем квадрат вокруг цели
    int startX = std::max(0, targetX - current_area);
    int endX = std::min(length - 1, targetX + current_area);
    int startY = std::max(0, targetY - current_area);
    int endY = std::min(width - 1, targetY + current_area);

    std::cout << "Checking area from (" << startX << ", " << startY << ") to (" << endX << ", " << endY << ")" << std::endl;

    for (int x = startX; x <= endX; x++) {
        for (int y = startY; y <= endY; y++) {
            // Проверяем врагов
            for (auto& enemy : enemies) {
                int enemyX, enemyY;
                enemy.get_coords(enemyX, enemyY);
                if (enemyX == x && enemyY == y && !enemy.is_dead()) {
                    enemy.take_damage(damage);
                    hit_anything = true;
                    std::cout << "Splash hit enemy at (" << x << ", " << y << ") for " << damage << " damage" << std::endl;
                }
            }

            // Проверяем атакующую башню
            int attackTowerX, attackTowerY;
            attack_tower.get_coords(attackTowerX, attackTowerY);
            if (attackTowerX == x && attackTowerY == y && !attack_tower.is_death()) {
                attack_tower.take_damage(damage);
                hit_anything = true;
                std::cout << "Splash hit attack tower at (" << x << ", " << y << ") for " << damage << " damage" << std::endl;
            }

            // Проверяем башню-спавнер
            int spawnTowerX, spawnTowerY;
            tower.get_coords(spawnTowerX, spawnTowerY);
            if (spawnTowerX == x && spawnTowerY == y) {
                tower.take_damage(damage);
                hit_anything = true;
                std::cout << "Splash hit spawn tower at (" << x << ", " << y << ") for " << damage << " damage" << std::endl;
            }
        }
    }

    if (!hit_anything) {
        std::cout << "Splash spell cast but hit nothing in the area" << std::endl;
        // НО ВСЕ РАВНО ВОЗВРАЩАЕМ TRUE - заклинание сработало, просто не было целей
    }

    std::cout << "=== SPLASH SPELL END ===" << std::endl;
    return true;  // Splash ВСЕГДА true если в пределах дистанции
}