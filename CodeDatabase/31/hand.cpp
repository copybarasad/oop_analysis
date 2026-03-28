#include "hand.hpp"
#include <cstring>
#include "direct_damage_spell.hpp"
#include "area_damage_spell.hpp"
#include "ally_call_spell.hpp"
#include "game/save_exceptions.hpp"
#include "spells/health_spell.hpp"
#include "trap_spell.hpp"
#include "upgrade_spell.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

const SpellInfo Hand::spell_prices[6] = {
    {"direct damage spell", 10},
    {"trap spell", 15},
    {"area damage spell", 20},
    {"ally call spell", 25},
    {"upgrade spell", 30},
    {"health spell", 12}
};

Spell* Hand::create_random_spell() {
    Spells spell_type = (Spells)(rand() % (int)Spells::TotalCount);
    switch(spell_type) {
        case Spells::direct_damage: return new DirectDamageSpell(*field);
        case Spells::area_damage: return new AreaDamageSpell(*field);
        case Spells::trap: return new TrapSpell(*field);
        case Spells::ally_call: return new AllyCallSpell(*field);
        case Spells::upgrade: return new UpgradeSpell();
        default: return new DirectDamageSpell(*field);
    }
}

Hand::Hand(Field &field, Player &player, int max_count, int initial_spells) : field(&field), player(player), max_count(max_count) {
    for (int i = 0; i < initial_spells; i++) {
        spells.push_back(create_random_spell());
    }
}

void Hand::show_hand() {
    std::cout << "\033[96m" << "═══════ Your Hand ═══════" << "\033[0m" << std::endl;
    for (int i = 0; i < spells.size(); i++) {
       spells[i]->show_info();
    }
}

void Hand::show_spells_count() {
    std::cout << "Spells: " << spells.size() << "/" << max_count;
    if (count_upgrades > 0) {
        std::cout << " [+" << count_upgrades << "]";
    }
}

void Hand::show_spells_str(int index) {
    for (int i = 0; i < spells.size(); i++) {
        if (i == index) {
            std::cout << "\033[7m";  
        }
        std::cout << spells[i]->get_color() << " " << spells[i]->get_abbrev() << " " << "\033[0m";
    }
    std::cout << std::endl;
}

void Hand::print_pricelist() {
    std::cout << "\033[92m" << "╔════════════════════════════╗" << "\033[0m" << std::endl;
    std::cout << "\033[92m" << "║" << "\033[0m" << "          pricelist         " << "\033[92m" << "║" << "\033[0m" << std::endl;
    for (int i = 0; i < 5; i++) {
        std::cout << "\033[92m" << "║" << "\033[0m" << " " << (i+1) << ") ";
        std::cout << spell_prices[i].name;
        int spaces = 23 - strlen(spell_prices[i].name);
        for (int j = 0; j < spaces; j++) std::cout << " ";
        std::cout << (player.get_score() < spell_prices[i].base_cost ? "\033[91m" : "\033[92m") << spell_prices[i].base_cost << "$" << "\033[92m" << " ║" << "\033[0m" << std::endl;
    }
    std::cout << "\033[92m" << "╚════════════════════════════╝" << "\033[0m" << std::endl;
}

void Hand::show_shop_str(int index) {
    const char* abbrevs[5] = {"dds", "ts", "ads", "acs", "us"};
    const char* colors[5] = {"\033[38;2;26;232;36m", "\033[38;5;51m", "\033[38;5;190m", "\033[38;5;200m", "\033[95m"};
    
    for (int i = 0; i < 5; i++) {
        if (i == index) {
            std::cout << "\033[7m";  
        }
        std::cout << colors[i] << " " << abbrevs[i] << " " << spell_prices[i].base_cost << "$" << " " << "\033[0m";
    }
    std::cout << std::endl;
}

void Hand::buy_spell(int spell_number) {
    if (spells.size() >= max_count) {
        return;
    }
    
    if (spell_number < 1 || spell_number > 5) {
        throw std::invalid_argument("incorrect spell");
    }
    
    int cost = spell_prices[spell_number - 1].base_cost;
    if (player.get_score() < cost) {
        return;
    }
    
    switch(spell_number) {
        case 1:
            spells.push_back(new DirectDamageSpell(*field));
            break;
        case 2:
            spells.push_back(new TrapSpell(*field));
            break;
        case 3:
            spells.push_back(new AreaDamageSpell(*field));
            break;
        case 4:
            spells.push_back(new AllyCallSpell(*field));
            break;
        case 5:
            spells.push_back(new UpgradeSpell());
            break;
    }
    player.decrease_score(cost);
}

bool Hand::spell_needs_cursor(int index) {
    if (index >= spells.size() || index < 0) return false;
    return spells[index]->needs_point();
}

bool Hand::prepare_spell(int index) {
    if (index >= spells.size() || index < 0) {
        return false;
    }
    
    if (spells[index]->is_upgrade()) {
        count_upgrades++;
        spells.erase(spells.begin() + index);
        return false;
    }
    
    return true;
}

int Hand::get_spell_area_size(int index) {
    if (index >= spells.size() || index < 0) return 1;
    return spells[index]->get_area_size();
}

int Hand::get_spell_radius(int index) {
    if (index >= spells.size() || index < 0) return 0;
    return spells[index]->get_radius();
}

Point Hand::get_player_pos() {
    return player.get_position();
}

bool Hand::use_spell(int index, Point point) {
    if (index >= spells.size() || index < 0) {
        return false;
    }
    
    for (int i = 0; i < count_upgrades; i++) {
        spells[index]->upgrade();
    }
    
    Point player_pos = get_player_pos();
    
    if (!spells[index]->needs_point()) {
        point = player_pos;
    }

    if (spells[index]->use(point, player_pos)) {
        count_upgrades = 0;
        last_pending_allies = spells[index]->get_pending_allies();
        spells.erase(spells.begin() + index);
        return true;
    }
    return false;
}

std::vector<Entity*> Hand::get_last_pending_allies() {
    return last_pending_allies;
}

int Hand::get_spell_count() const {
    return spells.size();
}

int Hand::base_spell_level = 1;

void Hand::remove_half_spells() {
    int n = spells.size() / 2;

    for (int i = 0; i < n; i++) {
        int idx = rand() % spells.size();
        delete spells[idx];
        spells.erase(spells.begin() + idx);
    }
}

void Hand::level_up_spells() {
    for (Spell* spell : spells) {
        spell->level_up();
    }
}

void Hand::level_up_base_spells() {
    base_spell_level++;
}

std::vector<Entity*> Hand::get_pending_allies(int index) {
    if (index >= spells.size() || index < 0) return {};
    return spells[index]->get_pending_allies();
}

void Hand::update_field(Field &new_field) {
    field = &new_field;
    for (Spell* spell : spells) {
        
    }
}

void Hand::transfer_spells_to(Hand &other_hand) {
    for (Spell* spell : spells) {
        other_hand.spells.push_back(spell);
    }
    spells.clear();
}

void Hand::show_shop_spell_info(int index) {
    if (index < 0 || index >= 5) return;
    std::cout << "Shop spell: " << spell_prices[index].name << " - Cost: " << spell_prices[index].base_cost << "$" << std::endl;

}

void Hand::show_hand_spell_info(int index) {
    if (index >= spells.size() || index < 0) return;
    spells[index]->show_info();
}

nlohmann::json Hand::save()
{
    nlohmann::json j = nlohmann::json::array();

    for (Spell *spell : spells) {
        j.push_back(spell->save());
    }
    return j;
}

Hand::Hand(nlohmann::json &j, Field &field, Player &player, int max_count)
    : field(&field), player(player), max_count(max_count)
{
    for (auto &spell_j : j) {
        std::string type = spell_j["type"];
        Spell *spell;
        if (type == "acs") spell = new AllyCallSpell(spell_j, field);
        else if (type == "ads") spell = new AreaDamageSpell(spell_j, field);
        else if (type == "dds") spell = new DirectDamageSpell(spell_j, field);
        else if (type == "hs") spell = new HealthSpell(spell_j, player);
        else if (type == "ts") spell = new TrapSpell(spell_j, field);
        else if (type == "us") spell = new UpgradeSpell(spell_j);
        else {
            throw CorruptedDataException("unknown spell type");
        }

        spells.push_back(spell);
    }
}