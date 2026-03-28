#include "directdamagespell.h"
#include "gamefield.h"
#include <iostream>

DirectDamageSpell::DirectDamageSpell(const std::string& name, 
                                   const std::string& description,
                                   int damage, int range, int mana_cost)
    : Spell(name, description, range, mana_cost), damage_(damage) {}

bool DirectDamageSpell::can_cast(const Field& field, int target_x, int target_y) const {
    if (!Spell::can_cast(field, target_x, target_y)) {
        return false;
    }
    
    const Player* player = field.get_player();
    if (player && player->is_at_position(target_x, target_y)) {
        return true;
    }
    
    if (field.has_enemy_at(target_x, target_y)) {
        return true;
    }
    
    if (field.has_enemy_tower_at(target_x, target_y)) {
        return true;
    }
    
    return false;
}

bool DirectDamageSpell::cast(Field& field, int target_x, int target_y) {
    if (!can_cast(field, target_x, target_y)) {
        std::cout << "Заклинание не может быть применено в этой позиции!" << std::endl;
        return false;
    }
    
    bool hit_something = false;

    const Player* player = field.get_player();
    if (player && player->is_at_position(target_x, target_y)) {
        const_cast<Player*>(player)->take_damage(damage_);
        std::cout << "Вражеская башня применяет '" << name_ << "' и наносит " << damage_ 
                  << " урона игроку!" << std::endl;
        std::cout << "Здоровье игрока: " << player->get_health() << std::endl;
        hit_something = true;
    }
    
    if (!hit_something) {
        auto& enemies = field.get_enemies();
        for (auto& enemy : enemies) {
            if (enemy->is_alive() && enemy->is_at_position(target_x, target_y)) {
                enemy->take_damage(damage_);
                std::cout << "Заклинание '" << name_ << "' нанесло " << damage_ 
                          << " урона врагу!" << std::endl;
                std::cout << "Здоровье врага: " << enemy->get_health() << std::endl;
                
                if (!enemy->is_alive()) {
                    std::cout << "Враг повержен!" << std::endl;
                    Player* player_ptr = const_cast<Player*>(field.get_player());
                    if (player_ptr) {
                        player_ptr->add_score(10);
                        player_ptr->add_enemy_defeated();
                    }
                }
                hit_something = true;
                break;
            }
        }
    }
    
    if (!hit_something) {
        auto& towers = field.get_enemy_towers();
        for (auto& tower : towers) {
            if (tower->is_alive() && tower->is_at_position(target_x, target_y)) {
                tower->take_damage(damage_);
                std::cout << "Заклинание '" << name_ << "' нанесло " << damage_ 
                          << " урона вражеской башне!" << std::endl;
                std::cout << "Здоровье башни: " << tower->get_health() << std::endl;
                
                if (!tower->is_alive()) {
                    std::cout << "Вражеская башня разрушена!" << std::endl;
                    Player* player_ptr = const_cast<Player*>(field.get_player());
                    if (player_ptr) {
                        player_ptr->add_score(20);
                    }
                }
                hit_something = true;
                break;
            }
        }
    }
    
    if (!hit_something) {
        std::cout << "Заклинание не достигло цели!" << std::endl;
        return false;
    }
    
    return true;
}