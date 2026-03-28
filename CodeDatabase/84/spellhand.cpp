#include "spellhand.h"
#include "directdamagespell.h"
#include "aoe_damagespell.h"
#include "trapspell.h"
#include "lightning_spell.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>

SpellHand::SpellHand(int max_size) : max_size_(max_size) {
    if (max_size_ <= 0) {
        max_size_ = 1;
    }
}

bool SpellHand::add_spell(std::unique_ptr<Spell> spell) {
    if (is_full()) {
        return false;
    }
    spells_.push_back(std::move(spell));
    return true;
}

bool SpellHand::remove_spell(int index) {
    if (index < 0 || index >= static_cast<int>(spells_.size())) {
        return false;
    }
    spells_.erase(spells_.begin() + index);
    return true;
}

void SpellHand::clear_hand() {
    spells_.clear();
}

Spell* SpellHand::get_spell(int index) const {
    if (index < 0 || index >= static_cast<int>(spells_.size())) {
        return nullptr;
    }
    return spells_[index].get();
}

int SpellHand::get_spell_count() const {
    return static_cast<int>(spells_.size());
}

int SpellHand::get_max_size() const {
    return max_size_;
}

bool SpellHand::is_full() const {
    return get_spell_count() >= max_size_;
}

void SpellHand::print_spells() const {
    std::cout << "=== ЗАКЛИНАНИЯ В РУКЕ ===" << std::endl;
    for (int i = 0; i < get_spell_count(); ++i) {
        Spell* spell = get_spell(i);
        if (spell) {
            std::cout << i + 1 << ". " << spell->get_name() 
                      << " | Дистанция: " << spell->get_range() 
                      << " | Мана: " << spell->get_mana_cost() 
                      << " | " << spell->get_short_info() << std::endl;
            std::cout << "   Описание: " << spell->get_description() << std::endl;
        }
    }
    std::cout << "Слотов: " << get_spell_count() << "/" << get_max_size() << std::endl;
}

std::unique_ptr<Spell> SpellHand::get_random_spell() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3);
    
    int spell_type = dis(gen);
    
    switch (spell_type) {
        case 0:
            return std::make_unique<DirectDamageSpell>(
                "Огненная стрела", "Наносит урон одному врагу", 25, 3, 10);
        case 1:
            return std::make_unique<AoEDamageSpell>(
                "Огненный шар", "Наносит урон по области", 15, 4, 2, 15);
        case 2:
            return std::make_unique<TrapSpell>(
                "Ловушка", "Устанавливает ловушку на поле", 20, 2, 3);
        case 3:
            return std::make_unique<LightningSpell>(
                "Удар молнии", "Наносит урон по области и оглушает", 20, 4, 1, 20);
        default:
            return std::make_unique<DirectDamageSpell>(
                "Огненная стрела", "Наносит урон одному врагу", 25, 3, 10);
    }
}

void SpellHand::remove_random_spells(int count_to_remove) {
    if (count_to_remove <= 0) return;
    if (count_to_remove >= get_spell_count()) {
        clear_hand();
        return;
    }

    std::vector<int> indices(get_spell_count());
    for (int i = 0; i < get_spell_count(); ++i) indices[i] = i;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);

    std::vector<int> indices_to_remove;
    for(int i = 0; i < count_to_remove; ++i) {
        indices_to_remove.push_back(indices[i]);
    }

    std::sort(indices_to_remove.rbegin(), indices_to_remove.rend());

    for (int idx : indices_to_remove) {
        remove_spell(idx);
    }
}