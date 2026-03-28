#pragma once
#include <vector>
#include "Spell.h"

class PlayerHand {
private:
    std::vector<Spell*> spells_owned;
    std::vector<std::pair<Spell*, int>> spells_in_shop;
    std::pair<int, int> damage_limit;
    std::pair<int, int> range_limits;
    std::pair<int, int> price_limits;
public:
    PlayerHand(int start_spell_count, int shop_spell_count, std::pair<int, int> damage, std::pair<int, int> range, std::pair<int, int> price);
    ~PlayerHand();
    PlayerHand() : damage_limit(0, 0), range_limits(0, 0), price_limits(0, 0) {}
    
    int get_spell_count() {return spells_owned.size(); }
    int get_shop_count() {return spells_in_shop.size(); }
    std::pair<Spell*, int> create_spell_with_price();
    int get_spell_price(int index) { return spells_in_shop[index].second; }
    void buy_spell(int shop_index);
    void remove_spell(int index);
    Spell::Type get_spell_type(int index) { return spells_owned[index]->get_type(); }

    std::string get_spell_info_str(int index);
    std::string get_shop_info_str(int index);

    std::vector<std::string> get_spells_data();
    std::vector<std::string> get_shop_data();

    bool use_spell(int index, Game &game, std::pair<int, int> caster, std::pair<int, int> target);

    json to_json();
    bool from_json(json& j);
    void remove_random_half();
};
