#include "hand.h"

Hand::Hand(unsigned int game_lvl): count(0), max_count(3 + game_lvl), upgrade(0) {
    get_spell(game_lvl);
};

Hand::~Hand(){
    for(unsigned int i = 0; i < count; i++){
        delete hand[i];
    }
};

void Hand::get_spell(unsigned int game_lvl){
    if(count == max_count) return;

    Spell* new_spell = nullptr;
    Randomiser& randomiser = Randomiser::get_randomiser();
    int random = randomiser.get_random(1, 5);
    switch(random){
        case 1:
            new_spell = new Direct_damage(game_lvl);
            break;
        case 2:
            new_spell = new Area_damage(game_lvl);
            break;
        case 3:
            new_spell = new Trap(game_lvl);
            break;
        case 4:
            new_spell = new Call_ally(game_lvl);
            break;
        case 5:
            new_spell = new Upgrade(game_lvl);
            break;
    }
    if(new_spell){
        hand.push_back(new_spell);
        count++;
    }
};

void Hand::delete_spells(){
    int to_delete = count/2;
    Randomiser& randomiser = Randomiser::get_randomiser();
    int index;
    for(unsigned int i = 0; i < to_delete; i++){
        index = randomiser.get_random(0, count - 1);
        hand.erase(hand.begin() + index);
        count--;
    }
    max_count++;
};

bool Hand::can_use_spell(){
    if(count == 0) return false;
    return true;
};

Spell_stats Hand::use_spell(unsigned int index){
    if(index >= count) index = count - 1;

    Spell* used_spell = hand[index];
    Spell_stats stats = used_spell->use_spell(this->upgrade);
    delete used_spell;

    hand.erase(hand.begin() + index);
    count--;
    return stats;
};

json Hand::to_json() const {
    json j;
    j["count"] = count;
    j["max_count"] = max_count;
    j["upgrade"] = upgrade;
        
    json spells_array = json::array();
    for (unsigned int i = 0; i < count; i++) {
        json spell_json;
        spell_type type = hand[i]->get_type();
        spell_json["type"] = static_cast<int>(type);
            
        spells_array.push_back(spell_json);
    }
    j["spells"] = spells_array;
        
    return j;
};
    
void Hand::from_json(const json& j, unsigned int game_lvl) {
    for (unsigned int i = 0; i < count; i++) {
        delete hand[i];
    }
    hand.clear();
        
    count = j["count"];
    max_count = j["max_count"];
    upgrade = j["upgrade"];
        
    const json& spells_array = j["spells"];
    for (const auto& spell_json : spells_array) {
        spell_type type = static_cast<spell_type>(spell_json["type"]);
        Spell* new_spell = nullptr;
            
        switch (type) {
            case spell_type::direct_damage:
                new_spell = new Direct_damage(game_lvl);
                break;
            case spell_type::area_damage:
                new_spell = new Area_damage(game_lvl);
                break;
            case spell_type::trap:
                new_spell = new Trap(game_lvl);
                break;
            case spell_type::call_ally:
                new_spell = new Call_ally(game_lvl);
                break;
            case spell_type::upgrade:
                new_spell = new Upgrade(game_lvl);
                break;
        }
            
        if (new_spell) {
            hand.push_back(new_spell);
        }
    }
};