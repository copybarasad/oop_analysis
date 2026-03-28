#include "direct_damage_spell.h"
#include <cmath>
#include "game.h"

void DirectDamageSpell::use(Game& game, GameField& field, Player& player, int target_row, int target_col) {
    if (!field.is_valid_position(target_row, target_col)) {
        std::cout << "Цель вне поля!\n";
        return;
    }

    int dist = std::abs(player.row() - target_row) + std::abs(player.col() - target_col);
    if (dist > radius_) {
        std::cout << "Цель вне радиуса заклинания!\n";
        return;
    }

    Enemy* enemy = game.get_enemy_at(target_row, target_col);
    if (enemy) {
        enemy->take_damage(damage_);
        std::cout << "Заклинание '" << name() << "' нанесло " << damage_
            << " урона врагу в клетке (" << target_row << ", " << target_col
            << "). Здоровье врага теперь: " << enemy->health() << "\n";

        if (!enemy->is_alive()) {
            std::cout << "Враг уничтожен!\n";
            field.clear_cell(target_row, target_col);
            game.remove_dead_enemies();
        }
    }
    else {
        std::cout << "В клетке (" << target_row << ", " << target_col << ") врагов нет.\n";
    }
}
