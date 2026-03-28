#include "PlayerHand.h"
#include <format>
#include <random>
#include <algorithm>
#include "AreaSpell.h"
#include "DirectSpell.h"
#include "TrapSpell.h"

PlayerHand::PlayerHand(int start_spell_count, int shop_spell_count, std::pair<int, int> damage, std::pair<int, int> range, std::pair<int, int> price)
: damage_limit(damage), range_limits(range),price_limits(price) {
    for (int i = 0; i < start_spell_count; i++) {
        spells_owned.push_back(create_spell_with_price().first);
    }
    for (int i = 0; i < shop_spell_count; i++) {
        spells_in_shop.push_back(create_spell_with_price());
    }
}

PlayerHand::~PlayerHand() {
    for (Spell* s : spells_owned) delete s;
    spells_owned.clear();
    for (auto &pr : spells_in_shop) delete pr.first;
    spells_in_shop.clear();
}

std::pair<Spell*, int> PlayerHand::create_spell_with_price() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> damage_dist(damage_limit.first, damage_limit.second);
    std::uniform_int_distribution<int> range_dist(range_limits.first, range_limits.second);
    std::uniform_int_distribution<int> price_dist(price_limits.first, price_limits.second);
    std::uniform_int_distribution<int> type_dist(0, 2);

    int damage = damage_dist(gen);
    int range = range_dist(gen);
    int price = price_dist(gen);
    Spell::Type type;
    Spell* spell_ptr = nullptr;
    switch (type_dist(gen)) {
        case 0:
            type = Spell::Type::Direct;
            spell_ptr = new DirectSpell(damage, range, type);
            break;
        case 1:
            type = Spell::Type::Area;
            spell_ptr = new AreaSpell(damage, range, type);
            break;
        case 2:
            type = Spell::Type::Trap;
            spell_ptr = new TrapSpell(damage, range, type);
            break;
    }
    return std::make_pair(spell_ptr, price);
}

std::string PlayerHand::get_spell_info_str(int index) {
    if(index < 0 || index >= spells_owned.size()) return "";
    return spells_owned[index]->get_type_str() + " | Урон:" + std::to_string(spells_owned[index]->get_damage())
           + " | Расстояние:" + std::to_string(spells_owned[index]->get_range());
}

std::string PlayerHand::get_shop_info_str(int index) {
    if(index < 0 || index >= spells_in_shop.size()) return "";
    return spells_in_shop[index].first->get_type_str() + " | Урон:" + std::to_string(spells_in_shop[index].first->get_damage())
           + " | Расстояние:" + std::to_string(spells_in_shop[index].first->get_range());
}

void PlayerHand::buy_spell(int shop_index) {
    Spell* bought_spell = spells_in_shop[shop_index].first;
    spells_owned.push_back(bought_spell);
    spells_in_shop.erase(spells_in_shop.begin() + shop_index);
    spells_in_shop.push_back(create_spell_with_price());
}

void PlayerHand::remove_spell(int index) {
    if (index >= 0 && index < static_cast<int>(spells_owned.size())) {
        delete spells_owned[index];
        spells_owned.erase(spells_owned.begin() + index);
    }
}

bool PlayerHand::use_spell(int index, Game& game, std::pair<int, int> caster, std::pair<int, int> target) {
    bool res = spells_owned[index]->use(game, caster, target);
    if (!res) return false;
    remove_spell(index);
    return true;
}

json PlayerHand::to_json() {
    json j;
    j["limits"] = {
        {"dmg_min", damage_limit.first}, {"dmg_max", damage_limit.second},
        {"rng_min", range_limits.first}, {"rng_max", range_limits.second},
        {"price_min", price_limits.first}, {"price_max", price_limits.second}
    };
    j["spells_owned"] = json::array();
    for(auto* s : spells_owned) j["spells_owned"].push_back(s->to_json());

    j["shop"] = json::array();
    for(auto& pair : spells_in_shop) {
        json item = pair.first->to_json();
        item["shop_price"] = pair.second;
        j["shop"].push_back(item);
    }
    return j;
}

bool PlayerHand::from_json(json& j) {
    for (Spell* s : spells_owned) delete s;
    spells_owned.clear();
    for (auto &pr : spells_in_shop) delete pr.first; spells_in_shop.clear();

    try {
        auto& l = j.at("limits");
        damage_limit = {l.at("dmg_min"), l.at("dmg_max")};
        range_limits = {l.at("rng_min"), l.at("rng_max")};
        price_limits = {l.at("price_min"), l.at("price_max")};
        for(auto& s_json : j.at("spells_owned")) {
            Spell* s = Spell::from_json(s_json);
            if(s) spells_owned.push_back(s);
        }

        for(auto& item_json : j.at("shop")) {
            Spell* s = Spell::from_json(item_json);
            int price = item_json.at("shop_price");
            if(s) spells_in_shop.push_back({s, price});
        }
        return true;
    } catch(...) { return false; }
}

void PlayerHand::remove_random_half() {
    if (spells_owned.empty()) return;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(spells_owned.begin(), spells_owned.end(), g);
    int count_to_remove = spells_owned.size() / 2;
    for (int i = 0; i < count_to_remove; ++i) {
        delete spells_owned.back();
        spells_owned.pop_back();
    }
}

std::vector<std::string> PlayerHand::get_spells_data() {
    std::vector<std::string> data;
    for (int i = 0; i < spells_owned.size(); i++) {
        data.push_back(std::to_string(i + 1) + ". " + get_spell_info_str(i));
    }
    return data;
}

std::vector<std::string> PlayerHand::get_shop_data() {
    std::vector<std::string> data;
    for (int i = 0; i < spells_in_shop.size(); i++) {
        data.push_back(std::to_string(i + 1) + ". " + get_shop_info_str(i) +
                       " | Цена: " + std::to_string(spells_in_shop[i].second));
    }
    return data;
}