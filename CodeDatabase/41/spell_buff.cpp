#include "spell_buff.h"
#include "player.h"
#include "game_field.h"
#include <iostream>

int Spell_buff::buff_stacks = 0;

Spell_buff::Spell_buff()
    : Spell_card("Магическое усиление", "Усиливает следующее заклинание", SPELL_BUFF_COST, 0){
}

bool Spell_buff::can_cast(const Player& player, int target_x, int target_y, const Game_field& field) const{
    (void)player;
    (void)target_x;
    (void)target_y;
    (void)field;
    return true; 
}

void Spell_buff::cast(Player& player, int target_x, int target_y, Game_field& field){
    (void)target_x;
    (void)target_y;
    (void)field;
    buff_stacks++;
    std::cout << player.get_name() << " накапливает магическую энергию!\n";
    std::cout << "Накоплено улучшений: " << buff_stacks << "\n";
    std::cout << "Следующее заклинание будет усилено!\n";
}

Spell_card* Spell_buff::clone() const{
    return new Spell_buff(*this);
}

void Spell_buff::enhance(){
    buff_stacks++; 
    std::cout << "Мощь улучшения возрастает! Текущий уровень: " << buff_stacks << "\n";
}

Target_type Spell_buff::get_target_type() const{
    return Target_type::SELF;
}

void Spell_buff::apply_buff(Spell_card* spell){
    if (!spell || buff_stacks == 0) return;
    std::cout << "Применяются " << buff_stacks << " улучшений к " << spell->get_name() << "!\n";
    for (int i = 0; i < buff_stacks; i++) {
        spell->enhance();
    }
    reset_buff();
}

void Spell_buff::reset_buff(){
    buff_stacks = 0;
}