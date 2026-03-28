#include "player_hand.hpp"
#include "spell_direct_damage.hpp"
#include "spell_area.hpp"
#include "spell_trap.hpp"

Player_hand::Player_hand(int hand_size): max_size(hand_size), gen(std::random_device{}()){
    if(max_size > 0) add_random_spell();
}

void Player_hand::add_random_spell(){
    if(!can_add_spell()) return;
    int choice = gen() % 3;
    switch (choice){
        case 0:
            spells.push_back(std::make_unique<Direct_damage_spell>("Lightning (Direct damage spell)", 40, 2));
            break;
        case 1:
            spells.push_back(std::make_unique<Area_spell>("Fireball (Area spell)", 30, 3));
            break;
        case 2:
            spells.push_back(std::make_unique<Trap_spell>("Trap", 75, 2));
            break;
    }
}

bool Player_hand::can_add_spell() const{
    return spells.size() < max_size;
}

void Player_hand::add_spell(std::unique_ptr<Spell> spell){
    if(can_add_spell()){
        spells.push_back(std::move(spell));
    }
}

std::unique_ptr<Spell> Player_hand::use_spell(int index){
    auto spell = std::move(spells[index]);
    spells.erase(spells.begin() + index);
    return spell;
}

void Player_hand::clear(){
    spells.clear();
}