#include "player.h"
#include "enemy.h"
#include "playing_field.h"
#include "entity_manager.h"
#include "enhancement_spell.h"
#include "spell.h"
#include "trap_spell.h"
#include "event_manager.h" 
#include <cmath>
#include <iostream>

Player::Player(int start_x, int start_y)
    : current_life(3), max_life(3), melee_damage(2), ranged_damage(1), score(0), position_x(start_x), position_y(start_y), is_alive(true), switching(false), melee_mode(true), spell_hand(3), enhancement_stacks(0) {}

void Player::move(int curr_x, int curr_y, const Playing_field& field) {
    if (!is_alive || switching){
        return;
    }

    int new_x = position_x + curr_x;
    int new_y = position_y + curr_y;
    
    if (new_x >= 0 && new_x < field.get_width() && 
        new_y >= 0 && new_y < field.get_length() &&
        field.get_cell(new_x, new_y).can_pass()) {
        
        position_x = new_x;
        position_y = new_y;

        EventManager::trigger(EventType::StateChanged, "Player moved", position_x, position_y);
    } else {
        std::cout << "Cannot move there! Wall or outside field." << std::endl;
    }
}

void Player::taking_damage(int damage){
    if (!is_alive){
        return;
    }
    if (damage <= 0) {
        std::cerr << "Game Over" << std::endl;
        throw std::invalid_argument("Invalid damage value");
    }
    current_life -= damage;
    EventManager::trigger(EventType::DamageTaken, "Player", damage, current_life);

    std::cout << "Player took " << damage << " damage! Life: " << current_life << "/" << max_life << std::endl;
    if (current_life <= 0){
        current_life = 0;
        is_alive = false;
        EventManager::trigger(EventType::UnitDied, "Player died");
    }
}

void Player::deals_damage(Enemy& enemy){
    if (!is_alive || switching || !enemy.get_alive()) return;

    int distance_x = std::abs(position_x - enemy.get_position_x());
    int distance_y = std::abs(position_y - enemy.get_position_y());
    int dmg_amount = 0;

    if(melee_mode){
        if (distance_x <= 1 && distance_y <= 1){
            dmg_amount = melee_damage;
            std::cout << "Melee attack! Damage: " << melee_damage << std::endl;
        }
    } else {
        if (distance_x <= 3 && distance_y <= 3){
            dmg_amount = ranged_damage;
            std::cout << "Ranged attack! Damage: " << ranged_damage << std::endl;
        }
    }

    if (dmg_amount > 0) {
        std::string weapon = melee_mode ? "Melee" : "Ranged";
        EventManager::trigger(EventType::DamageDealt, "Player (" + weapon + ")", dmg_amount);
        enemy.taking_damage(dmg_amount);
    }
}

void Player::switch_weapon() {
    melee_mode = !melee_mode;
    std::string mode = melee_mode ? "Melee" : "Ranged";
    EventManager::trigger(EventType::WeaponSwitched, "Switched to " + mode + " mode");
}

bool Player::is_melee_mode() const {
    return melee_mode;
}

void Player::getting_points(Enemy& enemy, int point) {
    if (!enemy.get_alive()) {
        score += point;
        EventManager::trigger(EventType::ScoreGained, "Enemy defeated", point, score);
    }
    else if (enemy.get_position_x() == -1 && enemy.get_position_y() == -1) {
        score += point;
        EventManager::trigger(EventType::ScoreGained, "Objective destroyed", point, score);
        std::cout << "Tower destroyed! +" << point << " points! Total: " << score << std::endl;
    }
    else {
        std::cout << "Cannot get points - enemy is still alive!" << std::endl;
    }
}

bool Player::spend_points(int amount) {
    if (amount < 0) {
        std::cout << "Cannot spend negative points!" << std::endl;
        return false;
    }

    if (score < amount) {
        std::cout << "Not enough points! Need " << amount << ", but only have " << score << std::endl;
        return false;
    }

    score -= amount;
    EventManager::trigger(EventType::ScoreSpent, "Purchase", amount, score);
    std::cout << "Spent " << amount << " points. Remaining: " << score << std::endl;
    return true;
}

bool Player::cast_spell(int spell_index, const Playing_field& field, int target_x, int target_y, std::vector<Enemy>& enemies, EntityManager& entity_manager) {
    if (!is_alive) return false;

    Spell* spell = spell_hand.get_spell(spell_index);
    if (!spell) {
        std::cout << "Invalid spell selection!" << std::endl;
        return false;
    }

    EventManager::trigger(EventType::SpellCast, spell->get_name());

    if (enhancement_stacks > 0) {
        spell->apply_enhancement(enhancement_stacks);
        reset_enhancement();
    }

    bool success = spell->cast(*this, field, target_x, target_y, enemies);
    
    if (success) {
        if (auto* trap_spell = dynamic_cast<TrapSpell*>(spell)) {
            auto trap_copy = trap_spell->clone();
            if (auto* cloned_trap = dynamic_cast<TrapSpell*>(trap_copy.get())) {
                trap_copy.release();
                entity_manager.add_trap_spell(std::unique_ptr<TrapSpell>(cloned_trap));
            }
        }
        
        spell_hand.remove_spell(spell_index);
        return true;
    } else {
        std::cout << "Spell failed!" << std::endl;
        return false;
    }
}

void Player::add_enhancement(int stacks) {
    enhancement_stacks += stacks;
    EventManager::trigger(EventType::EnhancementAdded, "Player enhanced", enhancement_stacks);
}

int Player::get_enhancement_stacks() const {
    return enhancement_stacks;
}

void Player::reset_enhancement() {
    enhancement_stacks = 0;
}

bool Player::get_alive() const {return is_alive;}
int Player::get_position_x() const {return position_x;}
int Player::get_position_y() const {return position_y;}
int Player::get_curr_life() const {return current_life;}
int Player::get_max_life() const {return max_life;}
int Player::get_score() const {return score;}
int Player::get_melee_damage() const {return melee_damage;}
int Player::get_ranged_damage() const {return ranged_damage;}
bool Player::get_melee_mode() const {return melee_mode;}
bool Player::get_switching() const {return switching;}
int Player::get_curr_damage() const {return melee_mode ? melee_damage : ranged_damage;}

Hand& Player::get_spell_hand() {
    return spell_hand;
}

const Hand& Player::get_spell_hand() const {
    return spell_hand;
}

void Player::set_position(int x, int y) {
    if (x < 0 || y < 0) {
        throw std::invalid_argument("Position coordinates cannot be negative");
    }
    position_x = x;
    position_y = y;
}

void Player::set_score(int new_score) {
    if (new_score < 0) {
        throw std::invalid_argument("Score cannot be negative");
    }
    score = new_score;
}

void Player::set_melee_mode(bool mode) {
    melee_mode = mode;
}

void Player::set_max_life(int new_max_life) {
    if (new_max_life <= 0) {
        throw std::invalid_argument("Max life must be positive");
    }
    max_life = new_max_life;
    if (current_life > max_life) {
        current_life = max_life;
    }
}

void Player::set_current_life(int new_life) {
    if (new_life < 0) {
        current_life = 0;
        is_alive = false;
    } else {
        current_life = std::min(new_life, max_life);
        is_alive = (current_life > 0);
    }
}

void Player::set_melee_damage(int new_damage) {
    if (new_damage < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    melee_damage = new_damage;
}

void Player::set_ranged_damage(int new_damage) {
    if (new_damage < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    ranged_damage = new_damage;
}

void Player::set_alive(bool alive) {
    is_alive = alive;
    if (!is_alive && current_life > 0) {
        current_life = 0;
    }
}

void Player::restore_health() {
    current_life = max_life;
    is_alive = true;
    EventManager::trigger(EventType::Healed, "Full Restore", current_life);
}

void Player::increase_spell_slots(int additional_slots) {
    if (additional_slots <= 0) {
        throw std::invalid_argument("Additional slots must be positive");
    }
    spell_hand.increase_max_size(additional_slots);
}