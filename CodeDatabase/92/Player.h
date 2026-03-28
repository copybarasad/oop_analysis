#pragma once
#include "Entity.h"
#include "PlayerHand.h"

class Player : public Entity {
public:
    Player(std::pair<int, int> position, int maxHealth, int damage, int far_attack_reach, PlayerHand& hand) :
    Entity(position, maxHealth, maxHealth, damage), far_attack_reach(far_attack_reach), hand(hand) {}

    void toggle_far_mode() { far_mode = !far_mode; }
    float attack() override;
    int get_points() { return points; }
    void add_points(int points) { this->points += points; }
    
    bool is_far_mode() { return far_mode; }
    int get_far_attack_reach() { return far_attack_reach; }
    void set_far_attack_reach(int reach) { far_attack_reach = reach; }
    void add_coins(int coins) {this->coins += coins; }
    void spend_coins(int coins) {this->coins -= coins; }
    int get_coins() { return coins; }

    int get_spell_count() {return hand.get_spell_count(); }
    int get_shop_count() {return hand.get_shop_count(); }
    int get_spell_price(int index) { return hand.get_spell_price(index); }
    void buy_spell(int shop_index) { hand.buy_spell(shop_index); }
    void remove_spell(int index) { hand.remove_spell(index); }
    Spell::Type get_spell_type(int index) { return hand.get_spell_type(index); }


    bool use_spell(int index, Game &game, std::pair<int, int> caster, std::pair<int, int> target)
    { return hand.use_spell(index, game, caster, target); }

    json to_json() override;
    bool from_json(json& j) override;

    void full_heal() { health = maxHealth; }
    void increase_max_health(int amount) { maxHealth += amount; health += amount; }
    void increase_damage(int amount) { damage += amount; }

    std::vector<std::string> get_player_spells_list() { return hand.get_spells_data(); }
    std::vector<std::string> get_player_shop_list() { return hand.get_shop_data(); }

private:
    int points = 0;
    bool far_mode = false;
    int far_attack_reach;
    int coins = 0;
    PlayerHand& hand;
};
