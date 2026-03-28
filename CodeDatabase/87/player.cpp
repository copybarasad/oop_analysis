#include "player.hpp"

Player::Player(int health, int close_damage_value, int ranged_damage_value, Position start_position, int hand_size):
    Character(health, close_damage_value, start_position), score(0), combat_style(Combat_style::CLOSE_COMBAT),
    close_damage(close_damage_value), ranged_damage(ranged_damage_value), hand(hand_size){}

void Player::switch_combat_style(Combat_style new_style){
    combat_style = new_style;
    if(new_style == Combat_style::CLOSE_COMBAT) damage = close_damage;
    else damage = ranged_damage;
}

Combat_style Player::get_combat_style() const{
    return combat_style;
}

int Player::get_score() const{
    return score;
}

void Player::add_score(int points){
    score += points;
}

void Player::take_damage(int damage_amount){
    Character::take_damage(damage_amount);
}

void Player::gain_spell(){
    if(hand.can_add_spell()){
        hand.add_random_spell();
    }
}

bool Player::buy_spell(int cost){
    if(score >= cost && hand.can_add_spell()){
        score -= cost;
        hand.add_random_spell();
        return true;
    }
    return false;
}

void Player::set_hand(Player_hand&& new_hand){
    hand = std::move(new_hand);
}

void Player::upgrade_damage(int amount){
    close_damage += amount;
    ranged_damage += amount;
    if(combat_style == Combat_style::CLOSE_COMBAT) damage = close_damage;
    else damage = ranged_damage;
}

void Player::upgrade_spell_damage(int amount){
    auto& spells = const_cast<std::vector<std::unique_ptr<Spell>>&>(hand.get_spells());
    for(auto& spell: spells){
        spell->add_damage(amount);
    }
}