#include "player.hpp"
#include "enemy.hpp"

#include "event_dispatcher.hpp"
#include "events.hpp"
#include "entity_snapshot_factory.hpp"

#include <algorithm>
#include <stdexcept>

Player::Player(int max_health, int base_damage)
    : Entity(max_health, base_damage),
      max_health_(max_health),
      score_(0),
      slowed_(false),
      ranged_mode_(false),
      kills_(0),
      hand_(3)
{
    if (max_health_ <= 0) { 
        throw std::invalid_argument("Player: max_health must be > 0!");
    }
    hand_.GenerateRandomSpell();
}

void Player::Heal(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("The Mad Scientist: heal amount must be > 0!");
    }
    if (health_ == 0)
        return;
    health_ = std::min(max_health_, health_ + amount);
}

void Player::AddScore(int delta) {
    if (delta < 0) {
        throw std::invalid_argument("The Mad Scientist: score delta must be > 0!");
    }
    score_ += delta;
}

int Player::getEffectiveDamage() const noexcept {
    return ranged_mode_ ? getDamage() / 2 : getDamage();
}

void Player::Attack(Entity& target) {
    target.TakeDamage(getEffectiveDamage());
}

void Player::ToggleMode() {
    ranged_mode_ = !ranged_mode_;

    if (ranged_mode_) {
        EventDispatcher::instance().notify(UiMessageEvent("🔫 Ranged mode enabled (damage halved)"));
    } else {
        EventDispatcher::instance().notify(UiMessageEvent("⚔️ Melee mode enabled (full damage)"));
    }
}

void Player::AddKills() {
    ++kills_;
    if (kills_ % 3 == 0 && !hand_.isFull()) {
        hand_.GenerateRandomSpell();
        EventDispatcher::instance().notify(UiMessageEvent("✨ Bonus spell gained for defeating 3 aliens!"));
    }
}

bool Player::SpendScore(int amount) {
    if (score_ < amount)
        return false;

    score_ -= amount;
    return true;
}
