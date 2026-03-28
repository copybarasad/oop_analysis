#include "player.h"
#include "game_controller.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include <iostream>

Player::Player(int health, int close_damage_value, int ranged_damage_value, 
               Position start_position, int max_spells)
    : Character(health, close_damage_value, start_position),
      score(0), 
      combat_style(Combat_style::CLOSE),
      close_damage(close_damage_value), 
      ranged_damage(ranged_damage_value),
      spell_hand(max_spells)
{
    damage = close_damage;
}

void Player::switch_combat_style(Combat_style new_style) {
    combat_style = new_style;
    damage = (combat_style == Combat_style::CLOSE) ? close_damage : ranged_damage;
}

Combat_style Player::get_combat_style() const {
    return combat_style;
}

int Player::get_score() const {
    return score;
}

void Player::add_score(int points) {
    score += points;
}

void Player::take_damage(int damage_amount) {
    Character::take_damage(damage_amount);
}

void Player::cast_spell(int spell_index, Game_controller& controller, const Position& target) {
    spell_hand.cast_spell(spell_index, controller, target);
}

void Player::on_enemy_defeated() {
    spell_hand.on_enemy_defeated();
    add_score(10);
}

void Player::try_get_new_spell() {
    if (spell_hand.can_get_new_spell()) {
        spell_hand.get_random_spell();
    }
}

void Player::display_spells() const {
    spell_hand.display_spells();
}

void Player::level_up() {
    std::cout << "=== LEVEL UP! ===\n";
    
    upgrade_close_damage(5);
    upgrade_ranged_damage(2);
    upgrade_spells(3);
    
    heal(max_health);
    std::cout << "Health restored to maximum: " << max_health << " HP\n";
    
    std::cout << "=================\n";
}

void Player::upgrade_spells(int damage_bonus) {
    auto& spells = spell_hand.get_spells();
    int upgraded_count = 0;
    
    for (auto& spell : spells) {
        if (auto* direct_spell = dynamic_cast<DirectDamageSpell*>(spell.get())) {
            int old_damage = direct_spell->get_damage();
            auto new_spell = std::make_unique<DirectDamageSpell>(
                direct_spell->get_name(),
                direct_spell->get_description(),
                direct_spell->get_range(),
                old_damage + damage_bonus
            );
            spell = std::move(new_spell);
            upgraded_count++;
            std::cout << "Spell " << spell->get_name() << " upgraded: " 
                      << old_damage << " -> " << old_damage + damage_bonus << " damage\n";
        }
        else if (auto* area_spell = dynamic_cast<AreaDamageSpell*>(spell.get())) {
            int old_damage = area_spell->get_damage();
            auto new_spell = std::make_unique<AreaDamageSpell>(
                area_spell->get_name(),
                area_spell->get_description(),
                area_spell->get_range(),
                old_damage + damage_bonus,
                area_spell->get_area_size()
            );
            spell = std::move(new_spell);
            upgraded_count++;
            std::cout << "Spell " << spell->get_name() << " upgraded: " 
                      << old_damage << " -> " << old_damage + damage_bonus << " damage\n";
        }
    }
    
    if (upgraded_count > 0) {
        std::cout << "Upgraded " << upgraded_count << " spells\n";
    } else {
        std::cout << "No spells to upgrade\n";
    }
}

void Player::upgrade_close_damage(int bonus) {
    int old_damage = close_damage;
    close_damage += bonus;
    if (combat_style == Combat_style::CLOSE) {
        damage = close_damage;
    }
    std::cout << "Close combat damage upgraded: " << old_damage << " -> " << close_damage << "\n";
}

void Player::upgrade_ranged_damage(int bonus) {
    int old_damage = ranged_damage;
    ranged_damage += bonus;
    if (combat_style == Combat_style::RANGED) {
        damage = ranged_damage;
    }
    std::cout << "Ranged combat damage upgraded: " << old_damage << " -> " << ranged_damage << "\n";
}