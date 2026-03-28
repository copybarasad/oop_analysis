#include <cmath>
#include "area_damage_spell.h"
#include "game.h"

void AreaDamageSpell::use(Game& game, GameField& field, Player& player, int target_row, int target_col) {
    if (!field.is_valid_position(target_row, target_col)) {
        std::cout << "Цель вне поля!\n";
        return;
    }

    std::cout << "Заклинание '" << name() << "' нанесло " << damage_
        << " урона по области с центром в (" << target_row << ", " << target_col << ")\n";

     for (int r = target_row - radius_; r <= target_row + radius_; ++r) {
        for (int c = target_col - radius_; c <= target_col + radius_; ++c) {
            if (!field.is_valid_position(r, c)) continue;
            Enemy* enemy = game.get_enemy_at(r, c);
            if (enemy) {
                enemy->take_damage(damage_);
                std::cout << " -> Враг в клетке (" << r << ", " << c
                    << ") получил " << damage_ << " урона. Осталось: " << enemy->health() << "\n";
                if (!enemy->is_alive()) {
                    std::cout << "   Враг в клетке (" << r << ", " << c << ") уничтожен!\n";
                    field.clear_cell(r, c);
                }
            }
        }
    }

    game.remove_dead_enemies();
}
