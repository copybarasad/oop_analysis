#include "direct_damage_spell.h"
#include "player.h"
#include "enemy_manager.h"
#include "game_field.h"
#include "enemy.h"
#include "enemy_tower.h"
#include "position.h"
#include <iostream>
#include <cmath>
#include <limits>

void DirectDamageSpell::Cast(Player& player, EnemyManager& enemies, GameField& field) {
    std::cout << "Выберите цель (x y): ";
    int tx, ty;
    if (!(std::cin >> tx >> ty)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неверный ввод! Заклинание отменено.\n";
        return;
    }

    Position target = {tx, ty};

    if (tx < 0 || tx >= field.GetWidth() || ty < 0 || ty >= field.GetHeight()) {
        std::cout << "Цель вне поля! Заклинание не использовано.\n";
        return;
    }

    int distance = std::abs(target.x - player.GetPosition().x) + std::abs(target.y - player.GetPosition().y);
    const int MAX_RANGE = 5;
    if (distance > MAX_RANGE) {
        std::cout << "Цель слишком далеко! (макс. " << MAX_RANGE << " клеток)\n";
        return;
    }

    bool hit = false;

    for (auto& e : enemies.GetEnemies()) {
        if (e.IsAlive() && e.GetPosition() == target) {
            int damage = 40;
            e.TakeDamage(damage);
            std::cout << "Магический удар нанёс " << damage << " урона врагу на (" << tx << "," << ty << ")!\n";
            hit = true;

            if (!e.IsAlive()) {
                std::cout << "Враг уничтожен!\n";
                player.AddScore(10);
            }
            break;
        }
    }

    for (auto& tower : enemies.GetTowers()) {
        if (tower.IsAlive() && tower.GetPosition() == target) {
            int damage = 40;
            tower.TakeDamage(damage);
            std::cout << "Магический удар нанёс " << damage << " урона башне на (" << tx << "," << ty << ")!\n";
            hit = true;

            if (!tower.IsAlive()) {
                std::cout << "Башня уничтожена! +20 очков!\n";
                player.AddScore(20);
            }
            break;
        }
    }

    if (!hit) {
        std::cout << "На этой клетке нет цели! Заклинание не использовано.\n";
        return;
    }

}