#include "Player.h"
#include "GameField.h"
#include "SpellHand.h"
#include <iostream>
#include <cmath>

void Player::update(GameField& game_field) {
    add_mana(3);
    heal(1);
}

bool Player::move(GameField& game_field, int dx, int dy) {
    if(abs(dx) > 1 || abs(dy) > 1){
        throw std::invalid_argument("Invalid coords to move");
    }
    int x = get_x();
    int y = get_y();
    if(game_field.is_cell_passable(x + dx, y + dy)){
        game_field.move_entity(x, y, x + dx, y + dy);
        set_position(x + dx, y + dy);
        return true;
    }else if(game_field.is_cell_occupied(x + dx, y + dy)){
        attack(game_field, dx, dy);
        return true;
    }

    std::cout << "You cant move on" << "(x: " << x + dx << ", y: " << y + dy << ")" << std::endl;
    return false;
}

bool Player::attack(GameField& game_field, int dx, int dy){
    if(dx == 0 && dy == 0){
        std::cout << "ypu cant attack yourself"<< std::endl; 
        return false;
    }
    int damage = game_field.get_player()->get_weapon().get_damage();
    int to_x = get_x() + dx;
    int to_y = get_y() + dy;
    if(can_attack(game_field, dx, dy)){
        auto entity = game_field.get_cell(to_x, to_y).get_entity();
        if(entity){
            entity->take_damage(damage);
            std::cout << "Hit enemy on " << "(x: " << to_x << ", y: " << to_y << ")"<< std::endl;
            return true;
        }else {
            std::cout << "Attack missed, no target at (x: " << to_x << ", y: " << to_y << ")" << std::endl;
        }
    }else {
        std::cout << "Cant attack line of sight blocked" << std::endl;
    }
    return false;
}

bool Player::cast_spell(GameField& field, int spell_index, int target_x, int target_y){
    if(spell_index < 0 || spell_index >= get_spell_hand().get_size()){
        return false;
    }

    return get_spell_hand().cast_spell(field, *this, target_x, target_y, spell_index);
}

void Player::switch_weapon(WeaponType newWeaponType) {
    weapon_ = Weapon(newWeaponType);
    weapon_.set_damage(weapon_.get_damage()*damage_multiplier_);
    
    std::cout << "Switched to: " << weapon_.get_name() 
              << " (Damage: " << weapon_.get_damage() 
              << ", Range: " << weapon_.get_range() << ")" << std::endl;
}

bool Player::is_on_slow_cell(GameField& game_field){
    return game_field.get_cell(get_x(), get_y()).get_type() == CellType::SLOW_ZONE;
}

void Player::add_mana(int amount) {
    mana_ += amount;
    if (mana_ < 0) mana_ = 0;
    if (mana_ > max_mana_) mana_ = max_mana_;
}

bool Player::add_score(int points) {
    score_ += points;
    
    while (score_ >= score_for_next_level_) {
        level_++;
        score_ -= score_for_next_level_;
        
        score_for_next_level_ += LVL_UP_SCALE_EXP;
        
        upgrade_health(LVL_UP_SCALE_HP);
        upgrade_mana(MANA_PER_LEVEL);
        upgrade_damage(LVL_UP_SCALE_DAMAGE);

        heal_full();
        restore_mana_full();
        
        std::cout << "LEVEL UP! Now level " << level_ 
                  << "\nHealth: " << health_ << "/" << max_health_
                  << "\nDamage: " << weapon_.get_damage() << std::endl;
        return true;
    }

    return false;
}

void Player::upgrade_health(int amount) {
    max_health_ += amount;
    health_ += amount;
}

void Player::upgrade_mana(int amount) {
    max_mana_ += amount;
    mana_ += amount;
}

void Player::upgrade_damage(float multiplier) {
    damage_multiplier_ += multiplier;
    weapon_ = Weapon(weapon_.get_type());
    weapon_.set_damage(weapon_.get_damage() * damage_multiplier_);
}

bool Player::upgrade_spell(int spell_index) {
    auto& spell_hand = get_spell_hand();
    if (spell_index >= 0 && spell_index < spell_hand.get_size() && 
        spell_hand.get_spell(spell_index)) {
        
        ISpell* spell = spell_hand.get_spell(spell_index);
        return spell->upgrade();
    }
    return false;
}

void Player::show_level_up_menu() {

    bool level_up_complete = false;
    while (!level_up_complete) {

        std::cout << "=== UPGRADE ===" << std::endl;
        std::cout << "\n1. +15 Health" << std::endl;
        std::cout << "2. +10 Mana" << std::endl;
        std::cout << "3. +0.5 Damage multiplier" << std::endl;
        std::cout << "4. Upgrade Spell" << std::endl;
        std::cout << "5. Skip" << std::endl;

        int choice;
        std::cout << "Choice: ";
        std::cin >> choice;
        if (!std::cin) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        
        if (choice > 0 && choice < 4) level_up_complete = true;

        switch (choice) {
            case 1:
                upgrade_health(LVL_UP_SCALE_HP/2);
                break;
            case 2:
                upgrade_mana(MANA_PER_LEVEL);
                break;
            case 3:
                upgrade_damage(LVL_UP_SCALE_DAMAGE);
                break;
            case 4:
                if(show_spell_upgrade_menu()) level_up_complete = true;
                break;
            case 5:
                level_up_complete = true;
                std::cout << "Skipping upgrades" << std::endl;
                break;
            default:
                std::cout << "Invalid choice!" << std::endl;
                continue;
        }
    }
}

bool Player::show_spell_upgrade_menu() {
    auto& spell_hand = get_spell_hand();
    
    std::cout << "\n=== UPGRADE SPELL ===" << std::endl;
    
    for (int i = 0; i < spell_hand.get_size(); ++i) {
        if (spell_hand.get_spell(i)) {
            auto* spell = spell_hand.get_spell(i);
            std::cout << i + 1 << ". " << spell->get_name() 
                      << " (Lvl " << spell->get_level() << ")" 
                      << " -> " << spell->get_upgrade_info() << std::endl;  // Добавлена информация
        }
    }
    
    std::cout << "0. Back" << std::endl;
    
    int choice;
    std::cout << "Choice: ";
    std::cin >> choice;
    if (choice > 0 && upgrade_spell(choice - 1)) {
        std::cout << spell_hand.get_spell(choice - 1)->get_name() << " upgraded!" << std::endl;
        return true;
    }

    return false;
}

bool Player::spend_gold(int amount) {
    if (gold_ >= amount) {
        gold_ -= amount;
        return true;
    } else {
        std::cout << "not enough gold, need " << amount << ", but only have " << gold_ << std::endl;
        return false;
    }
}

void Player::spell_shop() {
    std::cout << "\n=== SPELL SHOP ===" << std::endl;
    std::cout << "Your gold: " << get_gold() << std::endl;
    std::cout << "Available spells:" << std::endl;
    std::cout << "1. Direct Damage - 30 gold" << std::endl;
    std::cout << "2. Area Damage - 40 gold" << std::endl;
    std::cout << "3. Trap - 25 gold" << std::endl;
    std::cout << "4. Summon Ally - 50 gold" << std::endl;
    std::cout << "5. Enhancement - 35 gold" << std::endl;
    std::cout << "6. Random Spell - 20 gold" << std::endl;
    std::cout << "0. Exit shop" << std::endl;
    
    int choice;
    std::cin >> choice;
    
    if (choice == 0) return;
    
    if (get_spell_hand().is_full()) {
        std::cout << "Spell hand is full!" << std::endl;
        return;
    }
    
    int cost = 0;
    SpellType spell_type = static_cast<SpellType>(choice - 1);
    
    switch(choice) {
        case 1: cost = 30; break;
        case 2: cost = 40; break;
        case 3: cost = 25; break;
        case 4: cost = 50; break;
        case 5: cost = 35; break;
        case 6: cost = 20; break;
        default: 
            std::cout << "Invalid choice!" << std::endl;
            return;
    }
    
    if (choice != 6) {
        bool has_duplicate = false;
        for (int i = 0; i < get_spell_hand().get_size(); ++i) {
            if (get_spell_hand().get_spell(i)->get_type() == spell_type) {
                has_duplicate = true;
                break;
            }
        }
        
        if (has_duplicate) {
            std::cout << "You already have this spell in your hand!" << std::endl;
            return;
        }
    }
    
    if (spend_gold(cost)) {
        if (choice != 6) {
            ISpell* spell;
            switch(spell_type) {
                case SpellType::DIRECT_DAMAGE: spell = new DirectDamageSpell(); break;
                case SpellType::AREA_DAMAGE: spell = new AreaDamageSpell(); break;
                case SpellType::TRAP: spell = new TrapSpell(); break;
                case SpellType::SUMMON: spell = new SummonSpell(); break;
                case SpellType::ENHANCEMENT: spell = new EnhancementSpell(); break;
            }
            get_spell_hand().add_spell(spell);
        } else {
            get_spell_hand().fill_random_spells(1);
        }
        std::cout << "Spell purchased successfully!" << std::endl;
    }
}

void Player::show_stats() const {
    std::cout << "=== PLAYER STATS ===" << std::endl;
    Entity::show_stats();
    
    std::cout << "Score: " << score_ 
              << "/" << score_for_next_level_
              << "\nLevel: " << level_ << std::endl;
}

std::string Player::serialize() const {
    std::string data;
    data += "PLAYER\n";
    data += Entity::serialize();
    data += "LEVEL " + std::to_string(level_) + "\n";
    data += "SCORE " + std::to_string(score_) + "\n";
    data += "SCORE_FOR_NEXT_LEVEL " + std::to_string(score_for_next_level_) + "\n";
    data += "GOLD " + std::to_string(gold_) + "\n";
    data += "MANA " + std::to_string(mana_) + "\n";
    data += "MAX_MANA " + std::to_string(max_mana_) + "\n";
    data += "DAMAGE_MULTIPLIER " + std::to_string(damage_multiplier_) + "\n";
    data += "SPELL_HAND " + std::to_string(spell_hand_->get_max_size()) + " ";
    for(int i = 0; i < spell_hand_->get_size(); i++){
        auto* spell = spell_hand_->get_spell(i);
        data += spell->get_name() + " " + std::to_string(spell->get_level()) + " ";
    }
    data += "\n\n";

    return data;
}