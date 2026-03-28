#include "console_renderer.h"
#include <iostream>

#include "field.h"


void ConsoleRenderer::draw_field(const Player& player, const GameState& game_state, const int current_turn) {

    std::cout << "\n=== ХОД " << current_turn << " ===\n";
    std::cout << "Игрок: Здоровье " << player.get_health()
          << " | Позиция (" << player.get_position().get_x() << "," << player.get_position().get_y() << ")"
          << " | Режим: " << (player.get_combat_mode() == CombatMode::MELEE ? "ближний бой" : "дальний бой")
          << " | Урон: " << player.get_current_damage()
          << " | Радиус: " << player.get_attack_range()
          << " | Очки: " << player.get_score() << "\n\n";


    // отрисовка поля
    for (int y = game_state.get_field().get_height() - 1; y >= 0; --y) { // система координат как в математике (у - вверх), а рисуем вниз
        std::cout << " ";
        for (int x = 0; x < game_state.get_field().get_width(); ++x) {
            Position pos(x, y);
            char symbol = '.';

            // Определяем символ для клетки
            if (pos == player.get_position()) {
                symbol = 'P';
            } else {

                bool has_entity = false;

                // Проверяем врагов
                for (const Enemy& enemy : game_state.get_enemies()) {
                    if (enemy.is_alive() && enemy.get_position() == pos) {
                        symbol = 'E';
                        has_entity = true;
                        break;
                    }
                }

                // Проверяем спавнеры
                if (!has_entity) {
                    for (const EnemySpawner& spawner : game_state.get_spawners()) {
                        if (spawner.is_alive() && spawner.get_position() == pos) {
                            symbol = 'S';
                            has_entity = true;
                            break;
                        }
                    }
                }

                // Проверяем башни
                if (!has_entity) {
                    for (const EnemyTower& tower : game_state.get_towers()) {
                        if (tower.is_alive() && tower.get_position() == pos) {
                            symbol = 'T';
                            has_entity = true;
                            break;
                        }
                    }
                }

                // Если нет сущностей, показываем тип клетки
                if (!has_entity) {
                    Cell cell = game_state.get_field().get_cell_at(pos);
                    if (cell.get_type() == CellType::OBSTACLE) symbol = '#';
                    else if (cell.get_type() == CellType::SLOW) symbol = '~';
                    else if (cell.get_type() == CellType::TRAP) symbol = 'k';
                }
            }

            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\nУсловные обозначения: P-игрок, E-враг, S-спавнер, #-стена, ~-замедление, k-ловушка, T-Башня\n";

}


