#include "draw.hpp"
#include <iostream>

void Draw::draw_field() {
    int height = field.get_height();
    int width = field.get_width();
    Point player_pos = player.get_position();
    int radius = cursor.radius;
    
    std::vector<Point> tower_positions;
    for (int tx = 0; tx < width; tx++) {
        for (int ty = 0; ty < height; ty++) {
            if (field[{tx, ty}].has_entity() && 
                field[{tx, ty}].get_entity().get_entity_type() == EntityType::EnemyTower) {
                tower_positions.push_back({tx, ty});
            }
        }
    }
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            bool in_radius = cursor.is_enabled && radius > 0 && std::abs(j - player_pos.x) <= radius && std::abs(i - player_pos.y) <= radius;
            bool in_cursor_area = cursor.is_enabled && 
                j >= cursor.position.x && j < cursor.position.x + cursor.area_size &&
                i >= cursor.position.y && i < cursor.position.y + cursor.area_size;
            
            bool in_tower_range = false;
            for (const Point& tower_pos : tower_positions) {
                if (std::abs(j - tower_pos.x) <= 2 && std::abs(i - tower_pos.y) <= 2) {
                    in_tower_range = true;
                    break;
                }
            }
            
            if (field[{j, i}].has_entity()) {
                Entity &entity = field[{j, i}].get_entity();
                int cur_hp = entity.get_health();
                char health_str[3];
                health_str[0] = (cur_hp / 10) % 10 + '0';
                health_str[1] = cur_hp % 10 + '0';
                health_str[2] = '\0';
                
                if (in_cursor_area) { 
                    std::cout << "\033[48;2;255;255;0m\033[38;2;0;0;0m" << health_str << "\033[0m";
                }
                else if (entity.get_entity_type() == EntityType::Player) {
                    std::cout << "\033[42m\033[30m" << health_str << "\033[0m";
                }
                else if (entity.get_entity_type() == EntityType::Ally) {
                    std::cout << "\033[48;5;148m\033[30m" << health_str << "\033[0m";
                }
                else if (entity.get_entity_type() == EntityType::Enemy) {
                    std::cout << "\033[41m\033[30m" << health_str << "\033[0m";
                }
                else if (entity.get_entity_type() == EntityType::EnemyBuilding) {
                    std::cout << "\033[44m\033[30m" << health_str << "\033[0m";
                }
                else if (entity.get_entity_type() == EntityType::EnemyTower) {
                    std::cout << "\033[48;2;245;7;158m\033[30m" << health_str << "\033[0m";
                }
            }
            else {
                if (in_cursor_area) { 
                    std::cout << "\033[38;5;226m▓▓\033[0m";
                }
                else if (field[{j, i}].has_trap()) {
                    std::cout << "\033[91m✕✕\033[0m";
                }
                else if (in_radius && field[{j, i}].get_type() == CellType::Default) {
                    std::cout << "\033[37m░░\033[0m";
                }
                else if (in_tower_range && field[{j, i}].get_type() == CellType::Default) {
                    std::cout << "\033[38;5;213m░░\033[0m";
                }
                else if (field[{j, i}].get_type() == CellType::Default) {
                    std::cout << "\033[32m░░\033[0m";
                }
                else if (field[{j, i}].get_type() == CellType::Impassible) {
                    std::cout << "  ";
                }
                else if (field[{j, i}].get_type() == CellType::Slow) {
                    std::cout << "\033[38;2;191;245;162m▒▒\033[0m";
                }
            }
        }
        std::cout << std::endl;
    }
}

 void Draw::draw_score() {
    bool is_melee = player.get_attack_type() == AttackType::Melee;
    int damage = is_melee ? player.get_melee_damage() : player.get_ranged_damage();
    int radius = is_melee ? 1 : player.get_ranged_radius();
    std::cout << "\033[92m" << "score: " << player.get_score() << "\033[0m"
                << "\033[97m" << "  attack type: " << "\033[0m\033[92m" << (is_melee ? "melee " : "ranged ") << "\033[0m"
                << "\033[97m" << " rad: " << "\033[0m\033[92m" << radius << " \033[0m"  
                << "\033[97m" << " dmg: " << "\033[0m\033[92m" << damage << "\033[0m"
                << "\033[97m" << "  ";
    hand.show_spells_count();
    std::cout << "\033[0m" << std::endl;
}

void Draw::print_pricelist() {
    std::cout << "\033[92m" << "╔════════════════════════════╗"  << "\033[0m" << std::endl;
    std::cout << "\033[92m" << "║" << "\033[0m" << "          pricelist         "  << "\033[92m" << "║" << "\033[0m"  << std::endl;
    std::cout << "\033[92m" << "║" << "\033[0m" << " 1) direct damage spell " << (player.get_score() < 10 ? "\033[91m" : "\033[92m") << "10$" << "\033[92m" << " ║" << "\033[0m" << std::endl;
    std::cout << "\033[92m" << "║" << "\033[0m" << " 2) area damage spell   " << (player.get_score() < 20 ? "\033[91m" : "\033[92m") << "20$" << "\033[92m" << " ║" << "\033[0m" << std::endl;
    std::cout << "\033[92m" << "║" << "\033[0m" << " 3) trap spell          " << (player.get_score() < 20 ? "\033[91m" : "\033[92m") << "20$" << "\033[92m" << " ║" << "\033[0m" << std::endl;
    std::cout << "\033[92m" << "║" << "\033[0m" << " 4) upgrade             " << (player.get_score() < 25 ? "\033[91m" : "\033[92m") << "25$" << "\033[92m" << " ║" << "\033[0m" << std::endl;
    std::cout << "\033[92m" << "║" << "\033[0m" << " 5) ally call spell     " << (player.get_score() < 25 ? "\033[91m" : "\033[92m") << "25$" << "\033[92m" << " ║" << "\033[0m" << std::endl;
    std::cout << "\033[92m" << "╚════════════════════════════╝"  << "\033[0m" << std::endl;
}

void Draw::print_info() {
    std::cout << "\033[92m" << "╔════════════════════════════════════════════════════════════════════╗" << "\033[0m" << std::endl;
    std::cout << "\033[92m" << "║" << "\033[0m" << "\033[95m" << " move using the arrows           " << "\033[0m" << "\033[91m" << " \"q\" - quit      " << "\033[0m" << "\033[93m" << "  \033[42m  \033[0m player       " << "\033[92m" << "║" << "\033[0m" << std::endl;
    std::cout << "\033[92m" << "║" << "\033[0m" << "\033[95m" << " press space to attack or cursor " << "\033[0m" << "\033[91m" << " \"h\" - help      " << "\033[0m" << "\033[93m" << "  \033[48;5;148m  \033[0m ally         " << "\033[92m" << "║" << "\033[0m" << std::endl;
    std::cout << "\033[92m" << "║" << "\033[0m" << "\033[95m" << " move cursor using the arrows    " << "\033[0m" << "\033[91m" << " \"1\" - show hand " << "\033[0m" << "\033[93m" << "  \033[41m  \033[0m enemy        " << "\033[92m" << "║" << "\033[0m" << std::endl;
    std::cout << "\033[92m" << "║" << "\033[0m" << "\033[95m" << " press enter to switch attack    " << "\033[0m" << "\033[91m" << " \"2\" - buy spell " << "\033[0m" << "\033[93m" << "  \033[44m  \033[0m enemy build  " << "\033[92m" << "║" << "\033[0m" << std::endl;
    std::cout << "\033[92m" << "║" << "\033[0m" << "\033[95m" << "                                 " << "\033[0m" << "\033[91m" << " \"3\" - use spell " << "\033[0m" << "\033[93m" << "  \033[48;2;245;7;158m  \033[0m enemy tower  " << "\033[92m" << "║" << "\033[0m" << std::endl;
    std::cout << "\033[92m" << "║" << "\033[0m" << "\033[95m" << "                                 " << "\033[0m" << "\033[91m" << " \"s\" - save game " << "\033[0m" << "\033[93m" << "                  " << "\033[92m" << "║" << "\033[0m" << std::endl;
    std::cout << "\033[92m" << "║" << "\033[0m" << "\033[95m" << "                                 " << "\033[0m" << "\033[91m" << " \"l\" - load game " << "\033[0m" << "\033[93m" << "                  " << "\033[92m" << "║" << "\033[0m" << std::endl;
    std::cout << "\033[92m" << "╚════════════════════════════════════════════════════════════════════╝" << "\033[0m" << std::endl;
}
