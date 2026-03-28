#include "spell_hand.hpp"
#include <random>

Hand::Hand(int max_size) : max_size(max_size){}

void Hand::addSpell(std::unique_ptr<Spell> spell){
    if(spells.size() == max_size){
        std::cout << "Spell hand is full, maximum: " << max_size << " spells" << std::endl;
        return;
    }
    spells.push_back(std::move(spell));
    //std::cout << "Spell " << spells.back()->get_name() << " added to your hand" << std::endl;
}

void Hand::useSpell(int index, Player& player, int cordX, int cordY, 
    std::vector<Enemy>& enemies, const GameField& field) {
    if(index < 0 || index >= spells.size()){
        std::cout << "Invalid index" << std::endl;
        return;
    } 
    std::cout << "Using spell: " << spells[index]->get_name() << std::endl;
    spells[index]->cast_spell(player, cordX, cordY, enemies, field);
    std::cout << "Spell " << spells[index]->get_name() << " consumed!" << std::endl;
    spells.erase(spells.begin() + index);
}

int Hand::get_maxSize() const {
    return max_size;
}

int Hand::get_spellCount() const {
    return spells.size();
}

bool Hand::isFull() const {
    return spells.size() == max_size;
}

void Hand::display_hand() const {
    std::cout << "----------------------------------" << std::endl;
    for(int i = 0; i < spells.size(); i++){
        std::cout << i + 1 << ") " << spells[i]->get_name() << " " << spells[i]->get_disc() << std::endl;
    }
    if (spells.size() != max_size){
        std::cout << "Free space in hand: " << max_size - spells.size() << std::endl;
    }
}

const Spell* Hand::getSpell(int index) const {
    if (index < 0 || index >= spells.size()) {
        return nullptr;
    }
    return spells[index].get();
}
void Hand::removeRandomSpell() {
    if (spells.empty()) return;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, spells.size() - 1);
    int index = dis(gen);
    spells.erase(spells.begin() + index);
}