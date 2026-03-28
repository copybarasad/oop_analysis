#include "spell.h"
#include "gamefield.h"
#include "player.h"
#include <cmath>
#include <iostream>

Spell::Spell(const std::string& name, const std::string& description, int range, int mana_cost)
    : name_(name), description_(description), range_(range), mana_cost_(mana_cost) {}

std::string Spell::get_name() const {
    return name_;
}

std::string Spell::get_description() const {
    return description_;
}

bool Spell::can_cast(const Field& field, int target_x, int target_y) const {
    return is_target_in_range(field, target_x, target_y);
}

int Spell::get_range() const {
    return range_;
}

int Spell::get_mana_cost() const {
    return mana_cost_;
}

bool Spell::is_target_in_range(const Field& field, int target_x, int target_y) const {
    const Player* player = field.get_player();
    if (!player) return false;
    
    int player_x = player->get_x();
    int player_y = player->get_y();
    
    int distance = std::abs(player_x - target_x) + std::abs(player_y - target_y);
    if (distance > range_) return false;
    
    return field.is_valid_position(target_x, target_y);
}

void Spell::apply_damage_to_target(Field& field, int target_x, int target_y, int damage) const {
    auto& enemies = field.get_enemies();
    for (auto& enemy : enemies) {
        if (enemy->is_alive() && enemy->is_at_position(target_x, target_y)) {
            enemy->take_damage(damage);
            std::cout << "Нанесено " << damage << " урона врагу в (" << target_x << ", " << target_y << ")!" << std::endl;
            
            if (!enemy->is_alive()) {
                std::cout << "Враг повержен!" << std::endl;
                Player* player = const_cast<Player*>(field.get_player());
                if (player) {
                    player->add_score(10);
                    player->add_enemy_defeated();
                }
            }
            return;
        }
    }
    
    auto& towers = field.get_enemy_towers();
    for (auto& tower : towers) {
        if (tower->is_alive() && tower->is_at_position(target_x, target_y)) {
            tower->take_damage(damage);
            std::cout << "Нанесено " << damage << " урона башне в (" << target_x << ", " << target_y << ")!" << std::endl;
            
            if (!tower->is_alive()) {
                std::cout << "Вражеская башня разрушена!" << std::endl;
                Player* player = const_cast<Player*>(field.get_player());
                if (player) {
                    player->add_score(20);
                }
            }
            return;
        }
    }
}

void Player::upgrade_health(int amount) {
    max_health_ += amount;
    health_ = max_health_;
    std::cout << "Максимальное здоровье увеличено до " << max_health_ << "!" << std::endl;
}

void Player::upgrade_mana(int amount) {
    max_mana_ += amount;
    mana_ = max_mana_;
    std::cout << "Максимальная мана увеличена до " << max_mana_ << "!" << std::endl;
}