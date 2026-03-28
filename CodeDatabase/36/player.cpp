#include "player.h"
#include <iostream>

Player::Player(int health, int damage, int max_spells)
    : Character(health, damage),
      is_ranged_(false),
      skip_move_(false),
      pos_{0,0},
      max_spells_(max_spells),
      enhancement_stacks_(0),
      max_enhancement_stacks_(0) {}

Player::Player(Player&& other) noexcept
    : Character(std::move(other)),
      is_ranged_(other.is_ranged_),
      skip_move_(other.skip_move_),
      pos_(other.pos_),
      spells_list_(std::move(other.spells_list_)),
      max_spells_(other.max_spells_),
      enhancement_stacks_(other.enhancement_stacks_),
      max_enhancement_stacks_(other.max_enhancement_stacks_) {
    other.is_ranged_ = false;
    other.skip_move_ = false;
    other.pos_ = {0,0};
    other.max_spells_ = 0;
    other.enhancement_stacks_ = 0;
}

Player& Player::operator=(Player&& other) noexcept {
    if (this != &other) {
        Character::operator=(std::move(other));
        is_ranged_ = other.is_ranged_;
        skip_move_ = other.skip_move_;
        pos_ = other.pos_;
        spells_list_ = std::move(other.spells_list_);
        max_spells_ = other.max_spells_;
        enhancement_stacks_ = other.enhancement_stacks_;
        max_enhancement_stacks_ = other.max_enhancement_stacks_;

        other.is_ranged_ = false;
        other.skip_move_ = false;
        other.pos_ = {0,0};
        other.max_spells_ = 0;
        other.enhancement_stacks_ = 0;
        other.max_enhancement_stacks_ = 0;
    }
    return *this;
}

std::pair<int,int> Player::GetPosition() const { return pos_; }
void Player::SetPosition(int r, int c) { pos_ = {r,c}; }

void Player::SwitchCombatMode() { is_ranged_ = !is_ranged_; }
bool Player::IsRanged() const { return is_ranged_; }
int Player::CurrentDamage() const { return is_ranged_ ? std::max(1, Damage()/2) : Damage(); }

void Player::SetSkipNextMove(bool v) { skip_move_ = v; }
bool Player::ShouldSkipMove() const { return skip_move_; }
void Player::ConsumedSkipMove() { skip_move_ = false; }

int Player::GetEnhancementStacks() const { return enhancement_stacks_; }
void Player::AddEnhancementStack() { ++enhancement_stacks_; }
void Player::UpgradeEnhancementStack() {
    ++enhancement_stacks_; 
    ++max_enhancement_stacks_; 
    
}
void Player::ConsumeEnhancementStack() {
    while (enhancement_stacks_ > max_enhancement_stacks_) {
        --enhancement_stacks_;
    }
}

void Player::SetEnhancementStacks(int stacks) {
    enhancement_stacks_ = stacks;
}

void Player::SetMaxEnhancementStacks(int max_stacks) {
    max_enhancement_stacks_ = max_stacks;
}

int Player::GetMaxEnhancementStacks() const {
    return max_enhancement_stacks_;
}

bool Player::AddSpell(std::unique_ptr<Spell> s) {
    if (static_cast<int>(spells_list_.size()) >= max_spells_) {
        std::cout << "Spell hand is full! Cannot add new spell.\n";
        return false;
    }
    spells_list_.push_back(std::move(s));
    std::cout << "New spell added!\n";
    return true;
}

int Player::GetSpellCount() const { return static_cast<int>(spells_list_.size()); }
const Spell* Player::GetSpell(int idx) const {
    if (idx < 0 || idx >= GetSpellCount()) return nullptr;
    return spells_list_[idx].get();
}

bool Player::CastSpell(int idx, Field& field, const std::pair<int,int>& target, std::vector<Enemy>& enemies, std::vector<EnemyTower>& towers, std::vector<EnemyBase>& bases) {
    if (idx < 0 || idx >= GetSpellCount()) return false;
    const Spell* spell_ptr = GetSpell(idx);
    if (!spell_ptr) return false;

    Spell* nonconst = const_cast<Spell*>(spell_ptr);
    bool result = nonconst->Cast(field, *this, target, enemies, towers, bases);

    if (result) {
        RemoveSpellAt(idx);
    }

    return result;
}

void Player::RemoveSpellAt(int idx) {
    if (idx < 0 || idx >= GetSpellCount()) return;
    spells_list_.erase(spells_list_.begin() + idx);
}

void Player::LevelUp() {
    SetHealth(MaxHealth());
    int count = GetSpellCount();
    int to_remove = count / 2;
    for (int i = 0; i < to_remove; ++i) {
        int idx = rand() % GetSpellCount();
        RemoveSpellAt(idx);
    }
}

void Player::UpgradeHealth() {
    SetMaxHealth(MaxHealth() + 1);
    SetHealth(MaxHealth());
}

void Player::UpgradeDamage() {
    IncreaseDamage(1);
}

void Player::UpgradeMaxSpells() {
    max_spells_ += 1;
}

void Player::DecreaseMaxSpells() {
    max_spells_ -= 1;
}

int Player::GetMaxSpells() const {
    return max_spells_;
}

