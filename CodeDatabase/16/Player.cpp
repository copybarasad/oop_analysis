#include "Player.h"
#include "Spells.h"
#include "GameTypes.h"

Player::Player(int start_x, int start_y, int health, int melee_damage, int ranged_damage)
    : Entity(start_x, start_y, health, melee_damage),
      stance_(CombatStance::kMelee),
      melee_damage_(melee_damage),
      ranged_damage_(ranged_damage),
      hand_(5),
      max_health_(health) {
          hand_.addSpell(std::make_unique<DirectDamageSpell>(25, 5));
      }

void Player::switchStance() {
    if (stance_ == CombatStance::kMelee) {
        stance_ = CombatStance::kRanged;
        damage_ = ranged_damage_;
    } else {
        stance_ = CombatStance::kMelee;
        damage_ = melee_damage_;
    }
}

CombatStance Player::getStance() const {
    return stance_;
}

int Player::getRange() const {
    if (stance_ == CombatStance::kMelee) return 1;
    return 5;
}

bool Player::isSlowed() const {
    return is_slowed_;
}

void Player::setSlowed(bool slowed) {
    is_slowed_ = slowed;
}

Hand& Player::getHand() {
    return hand_;
}

Enhancements& Player::getEnhancements() {
    return enhancements_;
}

void Player::addScore(int amount) {
    score_ += amount;
}

int Player::getScore() const {
    return score_;
}

bool Player::spendScore(int amount) {
    if (score_ >= amount) {
        score_ -= amount;
        return true;
    }
    return false;
}

void Player::healFull() {
    health_ = max_health_;
}

void Player::increaseMaxHealth(int amount) {
    max_health_ += amount;
    healFull();
}

void Player::increaseMeleeDamage(int amount) {
    melee_damage_ += amount;
    if (stance_ == CombatStance::kMelee) {
        damage_ = melee_damage_;
    }
}

void Player::save(std::ostream& os) const {
    Entity::save(os);
    os << (int)stance_ << " " << melee_damage_ << " " << ranged_damage_ << " " 
       << score_ << " " << max_health_ << " " << is_slowed_ << " ";
    
    enhancements_.save(os);

    os << hand_.getMaxSize() << " " << hand_.getSpellCount() << " ";
    for(size_t i = 0; i < hand_.getSpellCount(); ++i) {
        os << (int)hand_.getSpell(i)->getType() << " ";
    }
}

void Player::load(std::istream& is) {
    Entity::load(is);
    int stanceVal;
    is >> stanceVal >> melee_damage_ >> ranged_damage_ >> score_ >> max_health_ >> is_slowed_;
    stance_ = static_cast<CombatStance>(stanceVal);
    if (stance_ == CombatStance::kMelee) damage_ = melee_damage_;
    else damage_ = ranged_damage_;

    enhancements_.load(is);

    size_t handMax, handCount;
    is >> handMax >> handCount;
    hand_.setMaxSize(handMax);
    hand_.clear();

    for(size_t i = 0; i < handCount; ++i) {
        int typeVal;
        is >> typeVal;
        switch(static_cast<SpellType>(typeVal)) {
            case SpellType::kDirect: hand_.addSpell(std::make_unique<DirectDamageSpell>(25, 5)); break;
            case SpellType::kArea: hand_.addSpell(std::make_unique<AreaDamageSpell>(15, 2)); break;
            case SpellType::kTrap: hand_.addSpell(std::make_unique<TrapSpell>(40)); break;
            case SpellType::kSummon: hand_.addSpell(std::make_unique<SummonSpell>()); break;
            case SpellType::kEnhance: hand_.addSpell(std::make_unique<EnhancementSpell>()); break;
            default: hand_.addSpell(std::make_unique<DirectDamageSpell>(10,1)); break; 
        }
    }
}