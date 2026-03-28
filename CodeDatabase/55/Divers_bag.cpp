#include "Divers_bag.h"
#include "Player.h"
#include "Sea.h"
#include "Jet_spell.h"
#include "Mine_spell.h"
#include "Fishing_net.h"
#include <iostream>
#include <iomanip>
#include <random>
#include <memory>
using namespace std;


Divers_bag::Divers_bag(GameConfig& cfg) : config(cfg), max_size(cfg.spells_count){
    addRandomSpell();
}

void Divers_bag::addRandomSpell(){
    if ((spells_size + traps_size) >= max_size) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2);

    if(dis(gen) == 0){
        spells.push_back(std::make_unique<Jet_spell>(config));
    }else if(dis(gen) == 1){
        spells.push_back(std::make_unique<Mine_spell>(config));
    }else{
        traps.push_back(std::make_unique<Fishing_net>(config));
    }
    spells_size = (int)spells.size();
    traps_size = (int)traps.size();
}

bool Divers_bag::bagIsEmpty(){
    return spells.empty() && traps.empty();
}

int Divers_bag::getMaxSize(){
    return max_size;
}

void Divers_bag::setMaxSize(int amount){
    max_size = amount;
}

int Divers_bag::getSize(){
    return spells_size + traps_size;
}

bool Divers_bag::check_spell_index(int index){
    if (index >= (int)spells.size() || index < 0){
        cout << "\033[41mИндекс заклинания вне диапазона!\033[0m" << endl;
        return false;
    }
    return true;
}

bool Divers_bag::check_trap_index(int index){
    if ((index - (int)spells.size()) >= (int)traps.size() || (index - (int)spells.size()) < 0){
        cout << "\033[41mИндекс заклинания вне диапазона!\033[0m" << endl;
        return false;
    }
    return true;
}

bool Divers_bag::whatGet(int index){
    if(index < (int)spells.size() && check_spell_index(index)){
        return true;
    }else if(index >= (int)spells.size() && check_trap_index(index)){
        return false;
    }
    return false;
}

Spell* Divers_bag::getSpell(int index){
    if(whatGet(index)){
        return spells[index].get();
    }
    return nullptr;
}

Trap* Divers_bag::getTrap(int index){
    if (!whatGet(index)){
        return traps[index - (int)spells.size()].get();
    }
    return nullptr;
}

void Divers_bag::useSpell(int index){
    if(check_spell_index(index)){
        spells.erase(spells.begin() + index);
        spells_size = spells.size();
    }
}

int Divers_bag::useTrap(Position &pos){
    int index = 0;
    for(auto& trap: traps){
        if(trap->getPosition() == pos){
            int tmp = trap->getDamage();
            traps.erase(traps.begin() + index);
            return tmp;
        }
        index += 1;
    }
    traps_size = traps.size();
    return 0;
}

void Divers_bag::gift(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    if (dis(gen) <= 50){
        addRandomSpell();
    }
}

void Divers_bag::display_bag(){
    cout << "\033[43m=== Заклинания в вашей сумке ===\033[0m" << endl;
    
    if(bagIsEmpty()) {
        cout << "\033[43mСумка пуста!\033[0m" << endl;
        return;
    }
    
    for(int i = 0; i < (int)spells.size(); i++){
        cout << i << ": " << spells[i]->getSpellTypeString()
            << " \033[32m(Урон:\033[0m " << spells[i]->getDamage() 
            << "\033[32m, Стоимость:\033[0m " << spells[i]->getCost() 
            << "\033[32m, Дальность:\033[0m " << spells[i]->getRange() << "\033[32m)\033[0m" << endl;
    }
    cout << "\033[33m================================\033[0m" << endl;

    for(int j = 0; j < (int)traps.size(); j++){
        cout << j + spells.size()<< ": " << traps[j]->getFlagString()
            << " \033[32m(Урон:\033[0m " << traps[j]->getDamage() 
            << "\033[32m, Стоимость:\033[0m " << traps[j]->getCost() 
            << "\033[32m, Дальность:\033[0m " << traps[j]->getRange() << "\033[32m)\033[0m" << endl;
    }
    cout << "\033[33m================================\033[0m" << endl;
}

void Divers_bag::reset(){
    for(int i = 0; i < (int)spells.size()/2; i++){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, spells.size() - 1);

        spells.erase(spells.begin() + dis(gen));
    }
    for(int i = 0; i < (int)traps.size()/2; i++){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, traps.size() - 1);

        traps.erase(traps.begin() + dis(gen));
    }
    traps_size = traps.size();
    spells_size = spells.size();
    curr_bag_size = spells_size + traps_size;
}

void Divers_bag::saveTo(SaveFileRAII& out) const {
    config.saveTo(out);
    out.write(static_cast<int>(spells_size))
       .write(static_cast<int>(traps_size))
       .write(static_cast<int>(max_size))
       .write(static_cast<int>(curr_bag_size));

    for (const auto& spell : spells) {
        int spell_type = spell->getSpellType();
        out.write(static_cast<SpellFlag>(spell_type));
        spell->saveTo(out);
    }

    for (const auto& trap : traps) {
        int trap_type = trap->getFlag();
        out.write(static_cast<SpellFlag>(trap_type));
        trap->saveTo(out);
    }
}

void Divers_bag::loadFrom(SaveFileRAII& in) {
    spells.clear();
    traps.clear();
    config.loadFrom(in);
    in.read(static_cast<int&>(spells_size))
    .read(static_cast<int&>(traps_size))
    .read(static_cast<int&>(max_size))
    .read(static_cast<int&>(curr_bag_size));

    for (int i = 0; i < spells_size; ++i) {
        std::unique_ptr<Spell> spell;
        SpellFlag spell_type;
        in.read(static_cast<SpellFlag&>(spell_type));
        switch (spell_type) {
            case jet_flag:
                spell = std::make_unique<Jet_spell>(config);
                break;
            case mine_flag:
                spell = std::make_unique<Mine_spell>(config);
                break;
            default:
                throw std::runtime_error("\033[41mНеизвестный тип заклинания после загрузки: " + std::to_string(spell_type) + "\033[0m");
        }
        spell->loadFrom(in);
        spells.push_back(std::move(spell));
    }

    for (int i = 0; i < traps_size; ++i) {
        SpellFlag trap_type;
        in.read(static_cast<SpellFlag&>(trap_type));

        std::unique_ptr<Trap> trap;
        switch (trap_type) {
            case fish_net:
                trap = std::make_unique<Fishing_net>(config);
                break;
            default:
                throw std::runtime_error("\033[41mНеизвестный тип ловушки после загрузки: " + std::to_string(trap_type) + "\033[0m");
        }
        trap->loadFrom(in);
        traps.push_back(std::move(trap));
    }
}