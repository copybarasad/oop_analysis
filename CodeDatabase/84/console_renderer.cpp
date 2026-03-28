#include "console_renderer.h"
#include <iostream>
#ifdef _WIN32
#include <cstdlib>
#endif

void ConsoleRenderer::render(const Game& game, const std::string& controls_help) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    std::cout << "=== " << controls_help << " ===" << std::endl;
    const Field& field = game.get_field();
    std::cout << "=== ИГРОВОЕ ПОЛЕ ===" << std::endl;
    for (int y = 0; y < field.get_height(); y++) {
        for (int x = 0; x < field.get_width(); x++) {
            const Player* player = field.get_player();
            if (player && player->is_at_position(x, y)) {
                std::cout << "P ";
            } else if (field.has_trap_at(x, y)) {
                std::cout << "T ";
            } else {
                bool enemy_found = false;
                for (const auto& enemy : field.get_enemies()) {
                    if (enemy->is_alive() && enemy->is_at_position(x, y)) {
                        std::cout << "E ";
                        enemy_found = true;
                        break;
                    }
                }
                if (!enemy_found) {
                    for (const auto& tower : field.get_enemy_towers()) {
                        if (tower->is_alive() && tower->is_at_position(x, y)) {
                            std::cout << "W ";
                            enemy_found = true;
                            break;
                        }
                    }
                }
                if (!enemy_found) {
                    const Cell& cell = field.get_cell(x, y);
                    if (cell.get_type() == CellType::Wall) {
                        std::cout << "# ";
                    } else {
                        std::cout << ". ";
                    }
                }
            }
        }
        std::cout << std::endl;
    }
    const Player* player = field.get_player();
    if (player) {
        std::cout << "\n=== ИНФОРМАЦИЯ ОБ ИГРОКЕ ===" << std::endl;
        std::cout << "Здоровье: " << player->get_health() << "/" << player->get_max_health() << std::endl;
        std::cout << "Очки: " << player->get_score() << std::endl;
        std::cout << "Мана: " << player->get_mana() << "/" << player->get_max_mana() << std::endl;
        std::cout << "Позиция: (" << player->get_x() << ", " << player->get_y() << ")" << std::endl;
        const Weapon* weapon = player->get_current_weapon();
        if (weapon) {
            std::cout << "Оружие: " << weapon->get_name() << " (" << weapon->get_damage() << " урон, ";
            std::cout << (weapon->is_melee() ? "ближний бой" : "дальний бой") << ")" << std::endl;
        }
        player->print_spells();
        field.print_enemies_info();
        field.print_towers_info();
    }
}