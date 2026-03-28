#include "Hand.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "EnhanceSpell.h"
#include <iostream>
#include <chrono>

Hand::Hand(size_t maxSize)
    : maxSize_(maxSize), 
      rng_(static_cast<unsigned int>(
          std::chrono::system_clock::now().time_since_epoch().count())) {}

bool Hand::addSpell(std::unique_ptr<Spell> spell) {
    if (spells_.size() >= maxSize_) {
        std::cout << "Hand is full! Cannot add more spells.\n";
        return false;
    }
    
    if (spell) {
        spell->applyPermanentBonus(permanentBonuses_);
        spells_.push_back(std::move(spell));
        return true;
    }
    return false;
}

size_t Hand::getSpellCount() const {
    return spells_.size();
}

size_t Hand::getMaxSize() const {
    return maxSize_;
}

const Spell* Hand::getSpell(size_t index) const {
    if (index < spells_.size()) {
        return spells_[index].get();
    }
    return nullptr;
}

bool Hand::castSpell(size_t index, SpellContext& context) {
    if (index >= spells_.size()) {
        std::cout << "Invalid spell index!\n";
        return false;
    }
    
    Spell* spell = spells_[index].get();
    if (!spell) {
        return false;
    }

    if (enhancementState_.hasEnhancements() && spell->getType() != SpellType::ENHANCE) {
        std::cout << "[ENHANCED] Next spell bonus: +"  << enhancementState_.damageBonus 
                  << " damage, +" << enhancementState_.rangeBonus 
                  << " range, +" << enhancementState_.areaBonus 
                  << " area, +" << enhancementState_.summonBonus << " summons\n";
    }

    bool success = spell->cast(context, enhancementState_);

    if (success) {
        spells_.erase(spells_.begin() + index);
        std::cout << "Spell used and removed from hand.\n";
    }
    
    return success;
}

const EnhancementState& Hand::getEnhancementState() const {
    return enhancementState_;
}

std::unique_ptr<Spell> Hand::getRandomSpell(std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(0, 4);
    int spellType = dist(rng);
    
    switch (spellType) {
        case 0:
            return std::make_unique<DirectDamageSpell>();
        case 1:
            return std::make_unique<AreaDamageSpell>();
        case 2:
            return std::make_unique<TrapSpell>();
        case 3:
            return std::make_unique<SummonSpell>();
        case 4:
            return std::make_unique<EnhanceSpell>();
        default:
            return std::make_unique<DirectDamageSpell>();
    }
}

void Hand::removeSpellAt(size_t index) {
    if (index < spells_.size()) {
        spells_.erase(spells_.begin() + index);
    }
}

void Hand::clearAllSpells() {
    spells_.clear();
    enhancementState_.reset();
}

std::unique_ptr<Spell> Hand::createSpell(SpellType type, const std::string& name) {
    std::string finalName = name;
    switch (type) {
        case SpellType::DIRECT_DAMAGE:
            if (finalName.empty()) finalName = "Fireball";
            return std::make_unique<DirectDamageSpell>(40, 3, finalName);
        case SpellType::AREA_DAMAGE:
            if (finalName.empty()) finalName = "Meteor";
            return std::make_unique<AreaDamageSpell>(20, 4, 2, finalName);
        case SpellType::TRAP:
            if (finalName.empty()) finalName = "Bear Trap";
            return std::make_unique<TrapSpell>(25, 2, finalName);
        case SpellType::SUMMON:
            if (finalName.empty()) finalName = "Summon Ally";
            return std::make_unique<SummonSpell>(finalName);
        case SpellType::ENHANCE:
            if (finalName.empty()) finalName = "Empower";
            return std::make_unique<EnhanceSpell>(finalName);
        default:
            return std::make_unique<DirectDamageSpell>(40, 3, "Fireball");
    }
}

void Hand::addPermanentSpellBonus(const EnhancementState& bonus) {
    permanentBonuses_.rangeBonus += bonus.rangeBonus;
    permanentBonuses_.areaBonus += bonus.areaBonus;
    permanentBonuses_.damageBonus += bonus.damageBonus;
    permanentBonuses_.summonBonus += bonus.summonBonus;
    permanentBonuses_.enhanceStack += bonus.enhanceStack;
    
    for (auto& spell : spells_) {
        if (spell) {
            spell->applyPermanentBonus(bonus);
        }
    }
}

