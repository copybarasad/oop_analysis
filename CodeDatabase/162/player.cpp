#include "player.h"
#include "melee_fight.h"
#include "ranged_fight.h"
#include "spell.h"
#include <cstdlib>
#include <ctime>
#include <iostream>  

Player::Player() : health_(100), max_health_(100), base_damage_(0), points_(0),
                   kill_count_(0), buff_count_(0), buff_range_(0), buff_area_(0),
                   buff_trap_damage_(0), buff_summons_(0) {
    fight_strategy_ = std::make_unique<MeleeFight>();
    hand_ = std::make_unique<Hand>(5);
    srand(time(0));
    int type = rand() % 5;
    if (type == 0) hand_->AddSpell(std::make_unique<DirectDamageSpell>());
    else if (type == 1) hand_->AddSpell(std::make_unique<AreaDamageSpell>());
    else if (type == 2) hand_->AddSpell(std::make_unique<TrapSpell>());
    else if (type == 3) hand_->AddSpell(std::make_unique<SummonSpell>());
    else hand_->AddSpell(std::make_unique<BuffSpell>());
}

Player::Player(const Player& other) 
    : health_(other.health_), 
      max_health_(other.max_health_), 
      base_damage_(other.base_damage_),
      points_(other.points_),
      kill_count_(other.kill_count_),
      buff_count_(other.buff_count_),
      buff_range_(other.buff_range_),
      buff_area_(other.buff_area_),
      buff_trap_damage_(other.buff_trap_damage_),
      buff_summons_(other.buff_summons_) {
    if (dynamic_cast<MeleeFight*>(other.fight_strategy_.get())) {
        fight_strategy_ = std::make_unique<MeleeFight>();
    } else {
        fight_strategy_ = std::make_unique<RangedFight>();
    }
    hand_ = std::make_unique<Hand>(5); 
    for (const auto& spell : other.hand_->GetSpells()) {
        hand_->AddSpell(std::make_unique<DirectDamageSpell>());
    }
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        health_ = other.health_;
        max_health_ = other.max_health_;
        base_damage_ = other.base_damage_;
        points_ = other.points_;
        kill_count_ = other.kill_count_;
        buff_count_ = other.buff_count_;
        buff_range_ = other.buff_range_;
        buff_area_ = other.buff_area_;
        buff_trap_damage_ = other.buff_trap_damage_;
        buff_summons_ = other.buff_summons_;
        
        if (dynamic_cast<MeleeFight*>(other.fight_strategy_.get())) {
            fight_strategy_ = std::make_unique<MeleeFight>();
        } else {
            fight_strategy_ = std::make_unique<RangedFight>();
        }
        
        hand_ = std::make_unique<Hand>(5);
        for (const auto& spell : other.hand_->GetSpells()) {
            hand_->AddSpell(std::make_unique<DirectDamageSpell>());
        }
    }
    return *this;
}

Player::~Player() {}

int Player::GetHealth() const { return health_; }

void Player::TakeDamage(int damage) {
    health_ -= damage;
    if (health_ < 0) health_ = 0;
}

int Player::GetPoints() const { return points_; }

void Player::AddPoints(int points) { points_ += points; }

void Player::SwitchFightMode() {
    if (dynamic_cast<MeleeFight*>(fight_strategy_.get())) {
        fight_strategy_ = std::make_unique<RangedFight>();
    } else {
        fight_strategy_ = std::make_unique<MeleeFight>();
    }
}

bool Player::CanAttack(int distance) const {
    return fight_strategy_->CanAttack(distance);
}

int Player::GetDamage() const {
    return base_damage_ + fight_strategy_->GetDamage();
}

std::string Player::GetFightModeName() const {
    return fight_strategy_->GetModeName();
}

void Player::BuySpell() {
    if (points_ >= 1 && !hand_->IsFull()) {
        points_ -= 1;
        int type = rand() % 5;
        if (type == 0) hand_->AddSpell(std::make_unique<DirectDamageSpell>());
        else if (type == 1) hand_->AddSpell(std::make_unique<AreaDamageSpell>());
        else if (type == 2) hand_->AddSpell(std::make_unique<TrapSpell>());
        else if (type == 3) hand_->AddSpell(std::make_unique<SummonSpell>());
        else hand_->AddSpell(std::make_unique<BuffSpell>());
        std::cout << "Bought a spell!\n";
    } else {
        std::cout << "Not enough points or hand full!\n";
    }
}

void Player::GainSpellAfterKills() {
    if (kill_count_ >= 5) {
        kill_count_ -= 5;
        if (!hand_->IsFull()) {
            int type = rand() % 5;
            if (type == 0) hand_->AddSpell(std::make_unique<DirectDamageSpell>());
            else if (type == 1) hand_->AddSpell(std::make_unique<AreaDamageSpell>());
            else if (type == 2) hand_->AddSpell(std::make_unique<TrapSpell>());
            else if (type == 3) hand_->AddSpell(std::make_unique<SummonSpell>());
            else hand_->AddSpell(std::make_unique<BuffSpell>());
            std::cout << "Gained a spell from kills!\n";
        }
    }
}

void Player::KillEnemy() {
    kill_count_++;
    GainSpellAfterKills();
}

void Player::AddBuff() {
    buff_count_++;
    if (buff_count_ % 4 == 0) {
        buff_range_++;
        std::cout << "Range buff increased!\n";
    }
    if (buff_count_ % 3 == 0) {
        buff_area_++;
        std::cout << "Area buff increased!\n";
    }
    if (buff_count_ % 5 == 0) {
        buff_trap_damage_++;
        std::cout << "Trap damage buff increased!\n";
    }
    if (buff_count_ % 7 == 0) {
        buff_summons_++;
        std::cout << "Summons buff increased!\n";
    }
}

void Player::ApplyBuffs() {
    // Баффы уже применены 
}

void Player::ResetBuffs() {
    buff_range_ = 0;
    buff_area_ = 0;
    buff_trap_damage_ = 0;
    buff_summons_ = 0;
}

