#include "hand.h"
#include "player.h"
#include <iostream>

Hand::Hand(size_t max_spells) : max_size(max_spells){
}

bool Hand::add_spell(std::unique_ptr<Spell_card> spell){
    if (spells.size() >= max_size) {
        std::cout << "Рука заклинаний переполнена! Максимум " << max_size << " заклинаний.\n";
        return false;
    }
    spells.push_back(std::move(spell));
    std::cout << "Заклинание " << spells.back()->get_name() << " добавлено в руку.\n";
    return true;
}

bool Hand::remove_spell(size_t index){
    if (index >= spells.size()) {
        return false;
    }
    spells.erase(spells.begin() + index);
    return true;
}

void Hand::clear_hand(){
    spells.clear();
}

size_t Hand::get_spell_count() const{ 
    return spells.size(); 
}

size_t Hand::get_max_size() const{ 
    return max_size; 
}

const Spell_card* Hand::get_spell(size_t index) const{ 
    return (index < spells.size()) ? spells[index].get() : nullptr; 
}

Spell_card* Hand::get_spell(size_t index){
    return (index < spells.size()) ? spells[index].get() : nullptr;
}

bool Hand::cast_spell(size_t index, Player& player, int target_x, int target_y, Game_field& field){
    if (index >= spells.size()) {
        std::cout << "Неверный индекс заклинания!\n";
        return false;
    }
    
    if (player.get_mana() < spells[index]->get_mana_cost()){
        std::cout << "Недостаточно маны! Нужно " << spells[index]->get_mana_cost() 
                  << ", доступно " << player.get_mana() << "\n";
        return false;
    }
    
    if (!spells[index]->can_cast(player, target_x, target_y, field)){
        std::cout << "Невозможно применить заклинание в эту цель!\n";
        return false;
    }
    player.set_mana(player.get_mana() - spells[index]->get_mana_cost());
    spells[index]->cast(player, target_x, target_y, field);
    return true;
}

void Hand::display_hand() const{
    std::cout << "=== Заклинания в руке (" << spells.size() << "/" << max_size << ") ===\n";
    for (size_t i = 0; i < spells.size(); ++i) {
        std::cout << i + 1 << ". " << spells[i]->get_name() 
                  << " (мана: " << spells[i]->get_mana_cost() 
                  << ", радиус: " << spells[i]->get_range() << ")\n";
        std::cout << "   " << spells[i]->get_description() << "\n";
    }
    if (spells.empty()) {
        std::cout << "В руке нет заклинаний\n";
    }
    std::cout << "==============================\n";
}