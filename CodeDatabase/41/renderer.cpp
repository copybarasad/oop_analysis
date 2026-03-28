#include "renderer.h"
#include "game.h"
#include "game_field.h"
#include "player.h"
#include "enums.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

void Console_renderer::clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Console_renderer::display_message(const std::string& message) {
    std::cout << message << std::endl;
}

void Console_renderer::render(const Game& game) {
    clear_screen();
    
    auto field = game.get_field();
    auto player = game.get_player();
    
    if (!field || !player) {
        std::cout << "Игра не инициализирована!\n";
        return;
    }
    
    std::cout << "Уровень: " << game.get_current_level();
    std::cout << " Ходы: " << game.get_total_turns() << std::endl;
    std::cout << std::string(field->get_width() + 2, '-') << std::endl;
    
    for (int y = 0; y < field->get_height(); ++y) {
        std::cout << "|";
        for (int x = 0; x < field->get_width(); ++x) {
            auto cell = field->get_cell(x, y);
            auto player_pos = field->get_player_position();
            auto enemy_positions = field->get_enemy_positions();
            
            char symbol = '.';
            
            if (player_pos.first == x && player_pos.second == y) {
                symbol = 'P'; 
            } else {
                bool is_enemy = false;
                for (const auto& enemy_pos : enemy_positions) {
                    if (enemy_pos.first == x && enemy_pos.second == y) {
                        symbol = 'E'; 
                        is_enemy = true;
                        break;
                    }
                }
                
                if (!is_enemy) {
                    switch (cell.get_type()) {
                        case Cell_type::BLOCKED:
                            symbol = '#'; 
                            break;
                        case Cell_type::SLOW:
                            symbol = '~'; 
                            break;
                        case Cell_type::BUILDING:
                            symbol = 'B'; 
                            break;
                        case Cell_type::EMPTY:
                        default:
                            symbol = '.';
                            break;
                    }
                }
            }
            
            std::cout << symbol;
        }
        std::cout << "|" << std::endl;
    }
    std::cout << std::string(field->get_width() + 2, '-') << std::endl;
    
    std::cout << "Игрок: " << player->get_name() << std::endl;
    std::cout << "Здоровье: " << player->get_health() << "/" << player->get_max_health() << std::endl;
    std::cout << "Мана: " << player->get_mana() << "/" << player->get_max_mana() << std::endl;
    std::cout << "Режим боя: " << (player->get_combat_mode() == Combat_mode::MELEE ? "Ближний" : "Дальний") << std::endl;
    std::cout << "Очки: " << player->get_score() << std::endl;
    
    if (player->get_is_slowed()) {
        std::cout << "Состояние: ЗАМЕДЛЕН" << std::endl;
    }
    
    std::cout << "\nЗаклинания:" << std::endl;
    player->get_spell_hand().display_hand();
    
    std::cout << "\nУправление: WASD-движение, E-атака, Q-сменить режим, 1-5-заклинания" << std::endl;
    std::cout << "C-сохранить, J-загрузить, H-лечение, N-выучить заклинание, Z-выход" << std::endl;
}