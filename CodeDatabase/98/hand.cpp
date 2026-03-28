#include "hand.h"
#include "directdamage.h"
#include "areadamage.h"
#include <cstdlib>

Hand::Hand(int size): size_hand(size) {
    spells.clear();
} 

Hand::Hand(const Hand& other) : size_hand(other.size_hand) {
    spells.clear();
    for (const auto& spell : other.spells) {
        if (spell->GetName() == "Заклинание прямого урона")
            spells.push_back(std::make_unique<DirectDamage>(10, 5));
        else if (spell->GetName() == "Заклинание урона по площади")
            spells.push_back(std::make_unique<AreaDamage>(15, 5));
    }
}

Hand& Hand::operator=(const Hand& other) {
    if (this == &other) return *this;
    size_hand = other.size_hand;
    spells.clear();
    for (const auto& spell : other.spells) {
        if (spell->GetName() == "Заклинание прямого урона")
            spells.push_back(std::make_unique<DirectDamage>(10, 5));
        else if (spell->GetName() == "Заклинание урона по площади")
            spells.push_back(std::make_unique<AreaDamage>(15, 5));
    }
    return *this;
}

const std::vector<std::unique_ptr<ISpell>>& Hand::GetSpells() const {
    return spells;
}

bool Hand::ShowSpells() const {
    return !spells.empty();
}

SpellResult Hand::UseSpell(int index_spell, std::vector<Enemy>& enemies, 
                          int playerX, int playerY, 
                          const std::vector<std::pair<int,int>>& enemyPositions,
                          int targetX, int targetY) {
    
    SpellResult result;
    
    if(index_spell < 1 || index_spell > static_cast<int>(spells.size())) {
        result.success = false;
        result.message = "Неверный выбор заклинания";
        return result;
    }

    result = spells[index_spell - 1]->UseSpell(enemies, playerX, playerY, enemyPositions, targetX, targetY);
    
    if (result.success) {
        std::string spellName = spells[index_spell - 1]->GetName();
        spells.erase(spells.begin() + (index_spell - 1));
        result.message += "\n" + spellName + " исчезло после использования.";
    }
    
    return result;
}

bool Hand::AddSpell(std::unique_ptr<ISpell> spell){
    if(static_cast<int>(spells.size()) >= size_hand){
        return false;
    }

    spells.push_back(std::move(spell));
    return true;
}

bool Hand::AddRandomSpell(){
    if(static_cast<int>(spells.size()) >= size_hand){
        return false;
    }
    
    int randVal = std::rand() % 2;
    
    if(randVal == 0) {
        spells.push_back(std::make_unique<DirectDamage>(10, 5));
    } else {
        spells.push_back(std::make_unique<AreaDamage>(15, 5));
    }
    
    return true;
}

std::string Hand::RemoveHalfSpells(){
    if (spells.empty()) return "Нет заклинаний для удаления";

    int toRemove = spells.size() / 2;
    int removed = 0;
    
    for (int i = 0; i < toRemove; ++i) {
        if (!spells.empty()) {
            int index = std::rand() % spells.size();
            spells.erase(spells.begin() + index);
            removed++;
        }
    }

    return "Удалено " + std::to_string(removed) + " заклинаний";
}

void Hand::ClearSpells() {
    spells.clear();
}

void Hand::AddLoadedSpell(std::unique_ptr<ISpell> spell) {
    if ((int)spells.size() < size_hand)
        spells.push_back(std::move(spell));
}

std::string Hand::GetSpellsInfo() const {
    std::string result;
    for (size_t i = 0; i < spells.size(); ++i) {
        result += std::to_string(i + 1) + ". " + spells[i]->GetName() + "\n";
    }
    return result;
}