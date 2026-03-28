#include "player_hand.h"
#include "heal_spell.h"
#include "fireball_spell.h"
#include "teleport_spell.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h" 
#include <random>
#include <iostream>

PlayerHand::PlayerHand(size_t max_size) : max_size_(max_size) {
    TryAddSpell(CreateInitialSpell());  
}

PlayerHand::~PlayerHand() {
    for (auto* spell : spells_) delete spell;
}

PlayerHand::PlayerHand(const PlayerHand& other) 
    : max_size_(other.max_size_) {
    
    for (const auto* spell : other.spells_) {
        if (spell) {
            spells_.push_back(CreateSpellByType(spell->GetType()));
        }
    }
}

PlayerHand& PlayerHand::operator=(const PlayerHand& other) {
    if (this != &other) {
        Clear();
        
        max_size_ = other.max_size_;
        
        for (const auto* spell : other.spells_) {
            if (spell) {
                spells_.push_back(CreateSpellByType(spell->GetType()));
            }
        }
    }
    return *this;
}

void PlayerHand::Clear() {
    for (auto* spell : spells_) {
        delete spell;
    }
    spells_.clear();
}

const Spell* PlayerHand::GetSpell(size_t index) const {
    if (index >= spells_.size()) return nullptr;
    return spells_[index];
}

void PlayerHand::RemoveSpell(size_t index) {
    if (index < spells_.size()) {
        delete spells_[index];
        spells_.erase(spells_.begin() + index);
    }
}

bool PlayerHand::AddSpecificSpell(SpellType type) {
    if (spells_.size() >= max_size_) {
        return false;
    }
    
    Spell* spell = CreateSpellByType(type);
    if (spell) {
        spells_.push_back(spell);
        return true;
    }
    return false;
}

bool PlayerHand::TryAddSpell(Spell* spell) {
    if (!spell || spells_.size() >= max_size_) {
        delete spell; 
        return false;
    }
    spells_.push_back(spell);
    return true;
}

bool PlayerHand::CastSpell(size_t index, Player& player, EnemyManager& enemies, GameField& field) {
    if (index >= spells_.size()) {
        std::cout << "Нет заклинания с таким номером!\n";
        return false;
    }
    
    spells_[index]->Cast(player, enemies, field);
    return true;
}

void PlayerHand::AddRandomSpell() {
    Spell* spell = CreateRewardSpell();  
    if (!TryAddSpell(spell)) {
        std::cout << "Рука полна — заклинание потеряно!\n";
        delete spell;
    }
}

Spell* PlayerHand::CreateInitialSpell() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 3);

    switch (dis(gen)) {
        case 1: return new HealSpell();
        case 2: return new FireballSpell();
        case 3: return new TeleportSpell();
        default: return new HealSpell();
    }
}

Spell* PlayerHand::CreateRewardSpell() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 6); 

    switch (dis(gen)) {
        case 1: return new HealSpell();
        case 2: return new FireballSpell();
        case 3: return new TeleportSpell();
        case 4: return new DirectDamageSpell();
        case 5: return new AreaDamageSpell();
        case 6: return new TrapSpell(); 
        default: return new HealSpell();
    }
}

Spell* PlayerHand::CreateSpellByType(SpellType type) const {
    switch (type) {
        case SpellType::Heal: return new HealSpell();
        case SpellType::Fireball: return new FireballSpell();
        case SpellType::Teleport: return new TeleportSpell();
        case SpellType::DirectDamage: return new DirectDamageSpell();
        case SpellType::AreaDamage: return new AreaDamageSpell();
        case SpellType::Trap: return new TrapSpell();  
        default: return nullptr;
    }
}