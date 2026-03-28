#include "Hand.h"
#include "Fireball.h" 
#include "Chaosbolt.h"
#include "Spikegrowth.h"
#include "Field.h"
#include "Player.h"

Hand::Hand() : gen(std::random_device{}()) {}

int Hand::cast_from_hand(int number, std::string coords, Field *field, int *counter_of_enemies, int *counter_of_towers, Player *player){
    if(number <= 0 || (size_t)number > this->list.size()){
        return 0;
    }
    int key;
    key = this->list[number - 1]->cast(coords, field, counter_of_enemies, counter_of_towers, player);
    if(key == 2){
        return 2;
    }
    if(key == 1){
        this->list.erase(this->list.begin() + (number - 1));
        return 1;
    }
    else{
        return 0;
    }
}

void Hand::add_spell(int upgrades){
    std::uniform_int_distribution<int> distrib_spell(0, 2);
    int spell_code = distrib_spell(this->gen);
    if(this->list.size() < (size_t)this->size){
        if(spell_code == 0){
            this->list.push_back(std::make_unique<Fireball>(upgrades));
        }
        else if(spell_code == 1){
            this->list.push_back(std::make_unique<Chaosbolt>(upgrades));
        }
        else if(spell_code == 2){
            this->list.push_back(std::make_unique<Spikegrowth>(upgrades));
        }
    }
}

void Hand::remove_spells(){
    int removes = this->list.size() / 2;
    for(int i = 0; i < removes; i++){
        std::uniform_int_distribution<int> distrib_spell(0, this->list.size() - 1);
        int spell_to_remove = distrib_spell(this->gen);
        this->list.erase(this->list.begin() + spell_to_remove);
    }
}

void Hand::print_hand() const {
    if (list.empty()) {
        std::cout << "РУКА: Пусто\n";
        return;
    }
    std::cout << "РУКА (" << list.size() << "/" << size << "): ";
    for (size_t i = 0; i < list.size(); ++i) {
        std::cout << (i + 1) << ". **" << list[i]->getName() << "** ";
    }
    std::cout << "\n";
}

void Hand::save(std::ofstream& ofs) const {
    ofs << list.size() << "\n";
    for (const auto& spell : list) {
        ofs << spell->getName() << "\n"; 
    }
}

void Hand::load(std::ifstream& ifs) {
    list.clear();
    size_t count;
    ifs >> count;
    
    for (size_t i = 0; i < count; i++) {
        std::string name;
        int upgrades =  0;
        if (!std::getline(ifs >> std::ws, name)) {
            break;
        }
        
        if (name == "Огненный шар") {
            list.push_back(std::make_unique<Fireball>(upgrades));
        } else if (name == "Снаряд хаоса") {
            list.push_back(std::make_unique<Chaosbolt>(upgrades));
        } else if (name == "Шипы") {
            list.push_back(std::make_unique<Spikegrowth>(upgrades));
        }
    }
}