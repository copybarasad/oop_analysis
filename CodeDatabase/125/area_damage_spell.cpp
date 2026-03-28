#include "area_damage_spell.h"
#include "player.h"        
#include "enemy_manager.h"
#include "game_field.h"   
#include "position.h"  
#include <iostream>
#include <cmath>
#include <vector>
#include <limits>

void AreaDamageSpell::Cast(Player& player, EnemyManager& enemies, GameField& field) {
    std::cout << "Выберите центр области 2x2 (x y): ";
    int cx, cy;
    if (!(std::cin >> cx >> cy)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неверный ввод! Заклинание отменено.\n";
        return;
    }

    Position center = {cx, cy};

    if (cx < 0 || cx >= field.GetWidth() || cy < 0 || cy >= field.GetHeight()) {
        std::cout << "Центр вне поля! Заклинание не использовано.\n";
        return;
    }

    int player_dist = std::abs(cx - player.GetPosition().x) + std::abs(cy - player.GetPosition().y);
    const int MAX_RANGE = 5;
    if (player_dist > MAX_RANGE) {
        std::cout << "Слишком далеко! (макс. " << MAX_RANGE << " клеток)\n";
        return;
    }

    std::vector<Position> area;
    for (int dx = -1; dx <= 0; ++dx) {
        for (int dy = -1; dy <= 0; ++dy) {
            int tx = cx + dx;
            int ty = cy + dy;
            if (tx >= 0 && tx < field.GetWidth() && ty >= 0 && ty < field.GetHeight()) {
                area.push_back({tx, ty});
            }
        }
    }

    if (area.empty()) {
        std::cout << "Область не помещается! Заклинание не использовано.\n";
        return;
    }

    const int DAMAGE = 25;
    int hit_count = 0;

    for (auto& e : enemies.GetEnemies()) {
        if (!e.IsAlive()) continue;
        for (const auto& pos : area) {
            if (e.GetPosition() == pos) {
                e.TakeDamage(DAMAGE);
                hit_count++;
                break; 
            }
        }
    }

    std::cout << "Взрыв нанес " << DAMAGE << " урона по области 2x2!\n";
    if (hit_count > 0) {
        std::cout << "Попадание по " << hit_count << " врагам!\n";
    } else {
        std::cout << "Никто не пострадал.\n";
    }

}