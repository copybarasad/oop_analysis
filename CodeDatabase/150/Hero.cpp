#include "Hero.hpp"
#include "SpellHand.hpp"
#include "GameState.hpp"
#include <iostream>

Hero::Hero() 
    : Character(0, 0, 100, 100, 20), 
      meleeDamage_(20), rangedDamage_(10), score_(0),
      mana_(100), maxMana_(100), currentAttackType_(AttackType::Melee), 
      isSlowed_(false), spellHand_(std::make_unique<SpellHand>(5)), 
      enhancementCount_(0), rangedRange_(3) 
{
    spellHand_->fillWithRandomSpells(1);
}

Hero::Hero(int startX, int startY, int health, int meleeDamage, 
           int rangedDamage, int mana, int maxMana, int rangedRange)
    : Character(startX, startY, health, health, meleeDamage), 
      meleeDamage_(meleeDamage), rangedDamage_(rangedDamage), score_(0),
      mana_(mana), maxMana_(maxMana), currentAttackType_(AttackType::Melee), 
      isSlowed_(false), spellHand_(std::make_unique<SpellHand>(5)), 
      enhancementCount_(0), rangedRange_(rangedRange)
{
    spellHand_->fillWithRandomSpells(1);
}

Hero::Hero(const Hero& other)
    : Character(other),
      meleeDamage_(other.meleeDamage_), 
      rangedDamage_(other.rangedDamage_),
      score_(other.score_),
      mana_(other.mana_),
      maxMana_(other.maxMana_),
      currentAttackType_(other.currentAttackType_),
      isSlowed_(other.isSlowed_),
      spellHand_(std::make_unique<SpellHand>(*other.spellHand_)),
      rangedRange_(other.rangedRange_)
{
}

Hero& Hero::operator=(const Hero& other) {
    if (this != &other) {
        Character::operator=(other);
        meleeDamage_ = other.meleeDamage_;
        rangedDamage_ = other.rangedDamage_;
        score_ = other.score_;
        mana_ = other.mana_;
        maxMana_ = other.maxMana_;
        currentAttackType_ = other.currentAttackType_;
        isSlowed_ = other.isSlowed_;
        spellHand_ = std::make_unique<SpellHand>(*other.spellHand_);
        rangedRange_ = other.rangedRange_;
    }
    return *this;
}

Hero::Hero(Hero&& other) noexcept
    : Character(std::move(other)),
      meleeDamage_(other.meleeDamage_), 
      rangedDamage_(other.rangedDamage_),
      score_(other.score_),
      mana_(other.mana_),
      maxMana_(other.maxMana_),
      currentAttackType_(other.currentAttackType_),
      isSlowed_(other.isSlowed_),
      spellHand_(std::move(other.spellHand_)),
      rangedRange_(other.rangedRange_)
{
    other.meleeDamage_ = 0;
    other.rangedDamage_ = 0;
    other.score_ = 0;
    other.mana_ = 0;
    other.maxMana_ = 0;
    other.enhancementCount_ = 0;
}

Hero& Hero::operator=(Hero&& other) noexcept {
    if (this != &other) {
        Character::operator=(std::move(other));
        
        meleeDamage_ = other.meleeDamage_;
        rangedDamage_ = other.rangedDamage_;
        score_ = other.score_;
        mana_ = other.mana_;
        maxMana_ = other.maxMana_;
        currentAttackType_ = other.currentAttackType_;
        isSlowed_ = other.isSlowed_;
        spellHand_ = std::move(other.spellHand_);
        enhancementCount_ = other.enhancementCount_;
        rangedRange_ = other.rangedRange_;
        
        other.meleeDamage_ = 0;
        other.rangedDamage_ = 0;
        other.score_ = 0;
        other.mana_ = 0;
        other.maxMana_ = 0;
        other.enhancementCount_ = 0;
    }
    return *this;
}

Hero::~Hero() = default;

int Hero::getDamage() const { 
    return (currentAttackType_ == AttackType::Melee) ? meleeDamage_ : rangedDamage_; 
}
    
bool Hero::isSlowed() const { 
    return isSlowed_; 
}

void Hero::addScore(int points) {
    score_ += points;
}

void Hero::switchAttackType() {
    currentAttackType_ = (currentAttackType_ == AttackType::Melee) 
        ? AttackType::Ranged 
        : AttackType::Melee;
}

void Hero::applySlowness() {
    isSlowed_ = true;
}
    
void Hero::removeSlowness() {
    isSlowed_ = false;
}

void Hero::gainMana(int amount) {
    if (amount > 0) {
        mana_ += amount;
        if (mana_ > maxMana_) {
            mana_ = maxMana_;
        }
    }
}

void Hero::heal(int heal){
    health_ += heal;
    validateHealth();
}

SpellHand& Hero::getSpellHand() {
    return *spellHand_;
}

const SpellHand& Hero::getSpellHand() const {
    return *spellHand_;
}

void Hero::addRandomSpell() {
    spellHand_->fillWithRandomSpells(1);
}

void Hero::restoreMana(int amount) {
    gainMana(amount);
}

void Hero::resetEnhancements() {
    enhancementCount_ = 0;
}

bool Hero::castSpell(int spellIndex, int targetX, int targetY, GameState& gameState) {
    if (spellIndex < 0 || spellIndex >= spellHand_->getMaxSize()) {
        return false;
    }
    auto* spell = spellHand_->getSpell(spellIndex);
    if (!spell || mana_ < spell->getManaCost()) {
        return false;
    }
    
    HeroSpellCaster caster(*this);
    
    if (spell->getType() == SpellType::Enhancement) {
        if (spell->cast(targetX, targetY, gameState, caster)) {
            mana_ -= spell->getManaCost();
            return true;
        }
        return false;
    }
    
    auto spellToCast = spell->clone();
    
    if (enhancementCount_ > 0) {
        spellToCast->applyEnhancements(enhancementCount_);
        resetEnhancements();
    }

    if (spellToCast->cast(targetX, targetY, gameState, caster)) {
        mana_ -= spell->getManaCost();
        return true;
    }
    
    return false;
}

void Hero::addEnhancement() { 
    enhancementCount_++; 
}

bool Hero::canBuySpell(int cost) const {
    return score_ >= cost && spellHand_->getSpellCount() < spellHand_->getMaxSize();
}

bool Hero::buySpell(int cost) {
    if (canBuySpell(cost)) {
        score_ -= cost;
        spellHand_->fillWithRandomSpells(1);
        return true;
    }
    return false;
}

void Hero::serialize(std::ostream& os) const {
    Character::serialize(os);
    os.write(reinterpret_cast<const char*>(&meleeDamage_), sizeof(meleeDamage_));
    os.write(reinterpret_cast<const char*>(&rangedDamage_), sizeof(rangedDamage_));
    os.write(reinterpret_cast<const char*>(&score_), sizeof(score_));
    os.write(reinterpret_cast<const char*>(&mana_), sizeof(mana_));
    os.write(reinterpret_cast<const char*>(&maxMana_), sizeof(maxMana_));
    
    int attackType = static_cast<int>(currentAttackType_);
    os.write(reinterpret_cast<const char*>(&attackType), sizeof(attackType));
    
    os.write(reinterpret_cast<const char*>(&isSlowed_), sizeof(isSlowed_));
    os.write(reinterpret_cast<const char*>(&enhancementCount_), sizeof(enhancementCount_));
    os.write(reinterpret_cast<const char*>(&rangedRange_), sizeof(rangedRange_));
    
    spellHand_->serialize(os);
}

void Hero::deserialize(std::istream& is) {
    Character::deserialize(is);
    is.read(reinterpret_cast<char*>(&meleeDamage_), sizeof(meleeDamage_));
    is.read(reinterpret_cast<char*>(&rangedDamage_), sizeof(rangedDamage_));
    is.read(reinterpret_cast<char*>(&score_), sizeof(score_));
    is.read(reinterpret_cast<char*>(&mana_), sizeof(mana_));
    is.read(reinterpret_cast<char*>(&maxMana_), sizeof(maxMana_));
    
    int attackType;
    is.read(reinterpret_cast<char*>(&attackType), sizeof(attackType));
    currentAttackType_ = static_cast<AttackType>(attackType);
    
    is.read(reinterpret_cast<char*>(&isSlowed_), sizeof(isSlowed_));
    is.read(reinterpret_cast<char*>(&enhancementCount_), sizeof(enhancementCount_));
    is.read(reinterpret_cast<char*>(&rangedRange_), sizeof(rangedRange_));
    
    spellHand_->deserialize(is);
}