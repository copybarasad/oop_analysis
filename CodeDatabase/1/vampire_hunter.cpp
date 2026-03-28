#include "vampire_hunter.h"
#include "field.h"
#include <stdexcept>
#include <cmath>

VampireHunter::VampireHunter(const std::string& name, const Position& position)
    : Character(name, 100, position), 
      research_progress_(0), 
      silver_bullets_(5),
      oak_stakes_(3),
      is_ranged_mode_(true),
      used_chapel_(false), 
      used_armory_(false),
      spellHand_(3) {
    spellHand_.addRandomStarterSpell();
}

void VampireHunter::takeDamage(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    health_ -= amount;  
    if (health_ < 0) {
        health_ = 0;
    }
}

void VampireHunter::applyCellEffect(CellType cell_type) {
    switch (cell_type) {
        case CellType::CHAPEL:
            applyChapelEffect();
            break;
        case CellType::ARMORY:
            applyArmoryEffect();
            break;
        case CellType::VAMPIRE_LAIR:
            applyVampireLairEffect();
            break;
        default:
            break;
    }
}

void VampireHunter::doResearch() {
    if (research_progress_ < 3) {
        research_progress_++;
    }
}

bool VampireHunter::isReadyForFinalBlow() const {
    return research_progress_ >= 3;
}

int VampireHunter::getResearchProgress() const { 
    return research_progress_; 
}

void VampireHunter::switchCombatMode() {
    is_ranged_mode_ = !is_ranged_mode_;
}

bool VampireHunter::isRangedMode() const {
    return is_ranged_mode_;
}

int VampireHunter::getSilverBullets() const {
    return silver_bullets_;
}

int VampireHunter::getOakStakes() const {
    return oak_stakes_;
}

void VampireHunter::useSilverBullet() {
    if (silver_bullets_ <= 0) {
        throw std::runtime_error("No silver bullets left!");
    }
    silver_bullets_--;
}

void VampireHunter::useOakStake() {
    if (oak_stakes_ <= 0) {
        throw std::runtime_error("No oak stakes left!");
    }
    oak_stakes_--;
}
//lb3
int VampireHunter::getCurrentAttackDamage() const {
    int base_damage = 15 + (research_progress_ * 5);
    
    if (is_ranged_mode_) {
        return base_damage + attack_damage_bonus_;
    } else {
        return (base_damage * 1.35) + attack_damage_bonus_ ;
    }
}
//
bool VampireHunter::canAttackAtPosition(const Position& attack_position) const {  
    int distance = position_.manhattanDistance(attack_position);  
    
    if (is_ranged_mode_) {
        return distance <= 3 && silver_bullets_ > 0;
    } else {
        return distance == 1 && oak_stakes_ > 0;
    }
}

void VampireHunter::applyLibraryEffect() {
    bool already_visited = false;
    for (const auto& pos : visited_libraries_) {
        if (pos == position_) {
            already_visited = true;
            break;
        }
    }
    
    if (!already_visited) {
        visited_libraries_.push_back(position_);
    }
}

void VampireHunter::applyChapelEffect() {
    if (!used_chapel_) {
        health_ += 25; 
        if (health_ > 100) health_ = 100;
        used_chapel_ = true;
    }
}

void VampireHunter::applyArmoryEffect() {
    if (!used_armory_) {
        silver_bullets_ += 3;
        used_armory_ = true;
    }
}

void VampireHunter::applyVampireLairEffect() {
    takeDamage(10);
}

bool VampireHunter::hasVisitedLibrary(const Position& pos) const {
    for (const auto& visited_pos : visited_libraries_) {
        if (visited_pos == pos) {
            return true;
        }
    }
    return false;
}

bool VampireHunter::hasUsedChapel() const { 
    return used_chapel_; 
}

bool VampireHunter::hasUsedArmory() const { 
    return used_armory_; 
}

//lb3

// НОВЫЕ методы для прокачки
void VampireHunter::upgradeHealth(int amount) {
    max_health_ += amount;
    health_ = max_health_; // полное исцеление при улучшении
}

void VampireHunter::upgradeAttackDamage(int amount) {
    attack_damage_bonus_ += amount;
}

void VampireHunter::upgradeSpellPower(int amount) {
    spell_damage_bonus_ += amount;
}

void VampireHunter::upgradeAmmo(int bullets, int stakes) {
    silver_bullets_ += bullets;
    oak_stakes_ += stakes;
}
