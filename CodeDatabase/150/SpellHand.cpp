#include "SpellHand.hpp"
#include <iostream>
#include <algorithm>

SpellHand::SpellHand(int maxSize) 
    : maxSize_(maxSize), randomGenerator_(std::random_device{}()) {}

SpellHand::SpellHand(const SpellHand& other)
    : maxSize_(other.maxSize_), randomGenerator_(std::random_device{}()) {
    
    for (const auto& spell : other.spells_) {
        if (spell) {
            spells_.push_back(spell->clone());
        }
    }
}

SpellHand::SpellHand(SpellHand&& other) noexcept
    : spells_(std::move(other.spells_)),
      maxSize_(other.maxSize_),
      randomGenerator_(std::move(other.randomGenerator_)) {
    
    other.maxSize_ = 0;
}

SpellHand& SpellHand::operator=(const SpellHand& other) {
    if (this != &other) {
        SpellHand temp(other);
        swap(temp);           
    }
    return *this;
}

SpellHand& SpellHand::operator=(SpellHand&& other) noexcept {
    if (this != &other) {
        spells_ = std::move(other.spells_);
        maxSize_ = other.maxSize_;
        randomGenerator_ = std::move(other.randomGenerator_);
        
        other.maxSize_ = 0;
    }
    return *this;
}

void SpellHand::swap(SpellHand& other) {
    using std::swap;
    swap(spells_, other.spells_);
    swap(maxSize_, other.maxSize_);
    swap(randomGenerator_, other.randomGenerator_);
}


bool SpellHand::addSpell(std::unique_ptr<ISpell> spell) {
    if (spells_.size() >= maxSize_) {
        return false;
    }
    spells_.push_back(std::move(spell));
    return true;
}

bool SpellHand::removeSpell(int index) {
    if (index < 0 || index >= spells_.size()) {
        return false;
    }
    spells_.erase(spells_.begin() + index);
    return true;
}

void SpellHand::clear() {
    spells_.clear();
}

ISpell* SpellHand::getSpell(int index) const {
    if (index < 0 || index >= spells_.size()) {
        return nullptr;
    }
    return spells_[index].get();
}

int SpellHand::getSpellCount() const {
    return spells_.size();
}

int SpellHand::getMaxSize() const {
    return maxSize_;
}

void SpellHand::fillWithRandomSpells(int count) {
    while (spells_.size() < maxSize_ && count > 0) {
        auto newSpell = createRandomSpell();
        SpellType newSpellType = newSpell->getType();

        bool alradyExists = false;
        for (const auto& existingSpell : spells_){
            if (existingSpell->getType() == newSpellType){
                alradyExists = true;
                break;
            }
        }

        if (alradyExists){
            continue;
        }

        if (addSpell(std::move(newSpell)))
            count--;
    }
}

std::unique_ptr<ISpell> SpellHand::takeSpell(int index) {
    if (index < 0 || index >= spells_.size()) {
        return nullptr;
    }
    auto spell = std::move(spells_[index]);
    spells_.erase(spells_.begin() + index);
    return spell;
}

std::unique_ptr<ISpell> SpellHand::createRandomSpell() const {
    std::uniform_int_distribution<> dist(0, 4);
    int spellType = dist(randomGenerator_);
    
    switch (spellType) {
        case 0:
            return std::make_unique<DirectDamageSpell>(20, 3, 10);
        case 1:
            return std::make_unique<AreaDamageSpell>(15, 2, 2, 15);
        case 2:
            return std::make_unique<TrapSpell>(25, 2, 12);
        case 3:
            return std::make_unique<SummonSpell>(1, 1, 20);
        case 4:
            return std::make_unique<EnhancementSpell>(5, 1, 1, 1, 20);
        default:
            return std::make_unique<DirectDamageSpell>(20, 3, 10);
    }
}

void SpellHand::serialize(std::ostream& os) const {
    int spellCount = spells_.size();
    os.write(reinterpret_cast<const char*>(&spellCount), sizeof(spellCount));
    
    for (const auto& spell : spells_) {
        SpellType type = spell->getType();
        os.write(reinterpret_cast<const char*>(&type), sizeof(type));
        spell->serialize(os);
    }
}

void SpellHand::deserialize(std::istream& is) {
    spells_.clear();
    int spellCount;
    is.read(reinterpret_cast<char*>(&spellCount), sizeof(spellCount));
    
    for (int i = 0; i < spellCount; ++i) {
        SpellType type;
        is.read(reinterpret_cast<char*>(&type), sizeof(type));
        
        std::unique_ptr<ISpell> spell;
        switch (type) {
            case SpellType::DirectDamage:
                spell = std::make_unique<DirectDamageSpell>();
                break;
            case SpellType::AreaDamage:
                spell = std::make_unique<AreaDamageSpell>();
                break;
            case SpellType::Trap:
                spell = std::make_unique<TrapSpell>();
                break;
            case SpellType::Summon:
                spell = std::make_unique<SummonSpell>();
                break;
            case SpellType::Enhancement:
                spell = std::make_unique<EnhancementSpell>();
                break;
        }
        
        if (spell) {
            spell->deserialize(is);
            spells_.push_back(std::move(spell));
        }
    }
}