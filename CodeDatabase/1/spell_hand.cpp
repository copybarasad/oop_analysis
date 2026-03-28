#include "spell_hand.h"
#include "vampire_hunter.h"
#include "holy_water_spell.h"
#include "light_beam_spell.h"
#include "sacred_glow_spell.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>

SpellHand::SpellHand(int maxSlots) : maxSlots_(maxSlots) {
    spells_.reserve(maxSlots_);
}

SpellHand::~SpellHand() {
    clearSpells();
}

// Конструктор копирования (глубокое копирование)
SpellHand::SpellHand(const SpellHand& other) 
    : maxSlots_(other.maxSlots_) {
    
    spells_.reserve(maxSlots_);
    for (Spell* spell : other.spells_) {
        if (spell) {
            spells_.push_back(createSpellCopy(spell));
        }
    }
}

// Конструктор перемещения
SpellHand::SpellHand(SpellHand&& other) noexcept
    : spells_(std::move(other.spells_)), maxSlots_(other.maxSlots_) {
    
    other.spells_.clear();
    other.maxSlots_ = 0;
}

// Оператор присваивания с копированием
SpellHand& SpellHand::operator=(const SpellHand& other) {
    if (this != &other) {
        clearSpells();
        
        maxSlots_ = other.maxSlots_;
        spells_.reserve(maxSlots_);
        
        for (Spell* spell : other.spells_) {
            if (spell) {
                spells_.push_back(createSpellCopy(spell));
            }
        }
    }
    return *this;
}

// Оператор присваивания с перемещением
SpellHand& SpellHand::operator=(SpellHand&& other) noexcept {
    if (this != &other) {
        clearSpells();
        
        spells_ = std::move(other.spells_);
        maxSlots_ = other.maxSlots_;
        
        other.spells_.clear();
        other.maxSlots_ = 0;
    }
    return *this;
}

bool SpellHand::addSpell(Spell* spell) {
    if (!spell || isFull()) {
        delete spell;
        return false;
    }
    spells_.push_back(spell);
    return true;
}

bool SpellHand::removeSpell(int index) {
    if (index < 0 || index >= spells_.size()) {
        return false;
    }
    
    delete spells_[index];
    spells_.erase(spells_.begin() + index);
    return true;
}

Spell* SpellHand::getSpell(int index) const {
    if (index < 0 || index >= spells_.size()) {
        return nullptr;
    }
    return spells_[index];
}

bool SpellHand::isFull() const {
    return spells_.size() >= maxSlots_;
}

int SpellHand::getSpellCount() const {
    return spells_.size();
}

void SpellHand::addRandomStarterSpell() {
    if (!spells_.empty()) {
        // Уже есть заклинание - выходим
        return;
    }
    
    // Случайное заклинание из 3 возможных
    int randomType = rand() % 3;
    Spell* spell = nullptr;
    
    switch (randomType) {
        case 0: spell = new HolyWaterSpell(); break;
        case 1: spell = new LightBeamSpell(); break;
        case 2: spell = new SacredGlowSpell(); break;
    }
    
    if (spell) {
        tryAddSpell(spell);  // ✅ Теперь добавится и в историю
    }
}
int SpellHand::getMaxSlots() const {
    return maxSlots_;
}
// Приватные методы
void SpellHand::clearSpells() {
    for (Spell* spell : spells_) {
        delete spell;
    }
    spells_.clear();
}

Spell* SpellHand::createSpellCopy(Spell* original) const {
    if (!original) return nullptr;
    
    switch (original->getType()) {
        case SpellType::HOLY_WATER: return new HolyWaterSpell();
        case SpellType::LIGHT_BEAM: return new LightBeamSpell();
        case SpellType::SACRED_GLOW: return new SacredGlowSpell();
        default: return nullptr;
    }
}

 bool SpellHand::tryAddSpell(Spell* spell){
   if (!spell || isFull() || wasSpellEverObtained(spell->getType())) {
        delete spell;
        return false;
    }
    spells_.push_back(spell);
    obtainedSpellsHistory_.push_back(spell->getType());
    return true; 

 }
 bool SpellHand::wasSpellEverObtained(SpellType spell) const{
    return std::find(obtainedSpellsHistory_.begin(), 
                    obtainedSpellsHistory_.end(), spell) != obtainedSpellsHistory_.end();
 }