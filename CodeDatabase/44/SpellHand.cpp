#include <functional>
#include <iostream>
#include "SpellHand.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"

SpellHand::SpellHand(int max_hand_size) :
    max_hand_size_(max_hand_size) {}

int SpellHand::GetSpellCount() const { 
    return static_cast<int>(spells_.size()); 
}

const std::vector<std::unique_ptr<Spell>>& SpellHand::GetSpells() const {
    return spells_;
}


bool SpellHand::IsEmpty() const {
    return spells_.empty();
}

void SpellHand::AddSpell(const std::string& name) {
    if (name == "Заклинание ближнего боя") {
        spells_.push_back(std::make_unique<DirectDamageSpell>());
    }
    else if (name == "Заклинание урона по площади (2x2)") {
        spells_.push_back(std::make_unique<AreaDamageSpell>());
    }
    else if (name == "Заклинание ловушки") {
        spells_.push_back(std::make_unique<TrapSpell>());
    }
    else {
        std::cerr << "Неизвестный тип заклинания: " << name << "\n";
    }
}

void SpellHand::AddRandomSpell() {
    if (spells_.size() >= static_cast<size_t>(max_hand_size_)) {
        return;
    }
    static const std::vector<std::function<std::unique_ptr<Spell>()>> factories = {
        [](){ return std::make_unique<DirectDamageSpell>(); },
        [](){ return std::make_unique<AreaDamageSpell>(); },
        [](){ return std::make_unique<TrapSpell>(); } 
    };

    int index = rand() % factories.size();
    spells_.push_back(factories[index]());

    std::cout << "Вы получили заклинание: " << spells_.back()->GetName() << "\n";
}

bool SpellHand::ShowSpells() const {
    if (spells_.empty()) {
        std::cout << "У вас нет заклинаний.\n";
        return false;
    }

    std::cout << "Ваши заклинания:\n";
    for (size_t i = 0; i < spells_.size(); ++i) {
        std::cout << i << ") " << spells_[i]->GetName() << "\n";
    }
    return true;
}

bool SpellHand::UseSpell(size_t index,
                         std::vector<Enemy>& enemies, std::vector<EnemyBuilding>& enemy_buildings,
                         int px, int py, Field& field) {
    if (index >= spells_.size()) {
        std::cout << "Неверный индекс заклинания!" << "\n";
        return false;
    }
    if (spells_[index]->Use(enemies, enemy_buildings, px, py, field)) {
        spells_.erase(spells_.begin() + index);
        return true;
    }
    return false;
}

void SpellHand::RemoveHalfSpells() {
    int to_remove = spells_.size() / 2;

    for (int i = 0; i < to_remove; ++i) {
        int idx = rand() % spells_.size();
        spells_.erase(spells_.begin() + idx);
    }
}

void SpellHand::Clear() {
    spells_.clear();
}