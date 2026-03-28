#include "hand_spells.h"
#include "directed_spell.h"
#include "area_spell.h"
#include "trap_spell.h"
#include <iostream>
#include <utility>

std::vector<std::unique_ptr<Spell>> create_spell_collection() {
    std::vector<std::unique_ptr<Spell>> spells;
    
    spells.push_back(std::make_unique<DirectedSpell>(1, "Explosion", 5, 30, 3));
    spells.push_back(std::make_unique<AreaSpell>(2, "Fireball", 12, 2, 2, 15, 6));
    spells.push_back(std::make_unique<TrapSpell>(3, "Mine", 8, 30, 2));
    
    return spells;
}

nlohmann::json Hand::toJson() const {
    std::vector<nlohmann::json> spellsJson;
    for (const auto& spell : spells) {
        spellsJson.push_back(spell->toJson());
    }
    
    return {
        {"spells", spellsJson},
        {"max_size", max_size}
    };
}

void Hand::fromJson(const nlohmann::json& j) {
    max_size = j["max_size"];
    
    spells.clear();
    
    for (const auto& spellJson : j["spells"]) {
        int spellType = spellJson["type"];
        std::string name = spellJson["name"];
        int mana_cost = spellJson["mana_cost"];
        int damage = spellJson["damage"];
        
        std::unique_ptr<Spell> spell;
        
        switch (spellType) {
            case 1: {
                int range = spellJson["range"]; 
                spell = std::make_unique<DirectedSpell>(1, name, mana_cost, damage, range);
                break;
            }
            case 2: {
                int area_radius = spellJson["area_radius"];
                int effect_duration = spellJson["effect_duration"];
                int range = spellJson["range"];
                spell = std::make_unique<AreaSpell>(2, name, mana_cost, area_radius, effect_duration, damage, range);
                break;
            }
            case 3: {
                int trap_duration = spellJson["trap_duration"];
                spell = std::make_unique<TrapSpell>(3, name, mana_cost, damage, trap_duration);
                break;
            }
            default:
                continue;
        }
        
        spells.push_back(std::move(spell));
    }
}
Hand::Hand(){
    initilization_hand();
    max_size = 5;
}

Hand::Hand(int size){
    initilization_hand();
    max_size = size;
}

void Hand::initilization_hand(){
    auto spells_collection = create_spell_collection();
    int random_index = random_numb(0, spells_collection.size() - 1);
    spells.push_back(std::move(spells_collection[random_index]));
}

void Hand::print_spells(){
    for (size_t i = 0; i < spells.size(); i++){
        std::cout << i << ": " << spells[i]->getName() 
                  << " (тип: " << spells[i]->getType() 
                  << ", мана: " << spells[i]->getManaCost() << ")\n";
    }
}

Spell& Hand::getSpell(int num){
    if (num >= 0 && num < spells.size()) {
        return *spells[num];
    }
    throw std::out_of_range("Invalid spell index");
}

void Hand::addSpell(std::unique_ptr<Spell> spell){
    if (spells.size() >= max_size){
        std::cout << "Рука полна! Нельзя добавить заклинание.\n";
        return;
    }
    spells.push_back(std::move(spell));
}

void Hand::addRandomSpell() {
    if (spells.size() >= max_size) {
        std::cout << "Рука полна! Нельзя добавить заклинание.\n";
        return;
    }
    
    auto spells_collection = create_spell_collection();
    int random_index = random_numb(0, spells_collection.size() - 1);
    spells.push_back(std::move(spells_collection[random_index]));
    std::cout << "Добавлено заклинание: " << spells.back()->getName() << "\n";
}
bool Hand::SearchSpellTrue(const std::string& name_spell){
    for (size_t i = 0; i < spells.size(); i++){
        if (spells[i]->getName() == name_spell){
            return true;
        }
    } 
    return false;
}

void Hand::UpSpell(int i) {
    if (i < 0 || i >= static_cast<int>(spells.size())) {
        std::cout << "Неверный индекс заклинания: " << i << std::endl;
        return;
    }
    
    Spell* spell = spells[i].get();
    if (!spell) {
        std::cout << "Заклинание не найдено по индексу: " << i << std::endl;
        return;
    }

    int oldDamage = spell->getDamage();
    
    spell->setDamage(oldDamage * 1.5);
    
    std::cout << "Улучшено заклинание: " << spell->getName() 
              << " с " << oldDamage << " до " << spell->getDamage() << " урона" << std::endl;
}
void Hand::clearHand() {
    spells.clear();
    std::cout << "Рука очищена" << std::endl;
}
Spell* Hand::SearchSpell(const std::string& name_spell){
    for (size_t i = 0; i < spells.size(); i++){
        if (spells[i]->getName() == name_spell){
            return spells[i].get();
        }
    }
    return nullptr;
}

void Hand::removeSpell(const std::string& name_spell) {
    for (auto it = spells.begin(); it != spells.end(); ++it) {
        if ((*it)->getName() == name_spell) {
            spells.erase(it);
            return;
        }
    }
}

size_t Hand::getSpellCount(){
    return spells.size();
}