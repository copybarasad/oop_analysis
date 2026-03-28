#include "Player.h"
#include "Hand.h"
#include "EventSystem.h"
#include <algorithm>
#include <random>

Player::Player(int hp, int damage, int points)
    : hp_(hp), maxHp_(hp), damage_(damage), points_(points),
      isSlowed_(false), attackMode_(AttackMode::MELEE), hand_(5),
      rangedRange_(3), slowResistance_(0.0f) {}

int Player::getHp() const { return hp_; }
int Player::getMaxHp() const { return maxHp_; }
int Player::getDamage() const { return damage_; }
int Player::getPoints() const { return points_; }
bool Player::isAlive() const { return hp_ > 0; }
bool Player::isSlowed() const { return isSlowed_; }
AttackMode Player::getAttackMode() const { return attackMode_; }
const Hand& Player::getHand() const { return hand_; }
Hand& Player::getHand() { return hand_; }

void Player::setSlowed(bool slowed) { 
    if (slowed && slowResistance_ > 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.0f, 1.0f);
        
        if (dis(gen) > slowResistance_) {
            isSlowed_ = true;
        }
    } else {
        isSlowed_ = slowed;
    }
}

void Player::setAttackMode(AttackMode mode) { attackMode_ = mode; }

void Player::addPoints(int pts) { 
    points_ += pts; 
    
    GameEvent event;
    event.type = EventType::UPGRADE_APPLIED;
    event.actor = "Player";
    event.value = pts;
    event.details = "Получено очков: " + std::to_string(pts);
    EventSystem::getInstance().notify(event);
}

void Player::takeDamage(int dmg) { 
    int oldHp = hp_;
    hp_ = std::max(0, hp_ - dmg); 
    
    GameEvent event;
    event.type = EventType::PLAYER_DAMAGE_TAKEN;
    event.actor = "Player";
    event.value = dmg;
    event.details = "HP: " + std::to_string(oldHp) + " -> " + std::to_string(hp_);
    EventSystem::getInstance().notify(event);
}

void Player::heal(int amount) { 
    int oldHp = hp_;
    hp_ = std::min(maxHp_, hp_ + amount); 
    
    if (amount > 0) {
        GameEvent event;
        event.type = EventType::PLAYER_HEALED;
        event.actor = "Player";
        event.value = amount;
        event.details = "HP: " + std::to_string(oldHp) + " -> " + std::to_string(hp_);
        EventSystem::getInstance().notify(event);
    }
}

void Player::increaseDamage(int amount) {
    int oldDamage = damage_;
    damage_ += amount;
    
    GameEvent event;
    event.type = EventType::UPGRADE_APPLIED;
    event.actor = "Player";
    event.details = "Урон увеличен: " + std::to_string(oldDamage) + " -> " + std::to_string(damage_);
    EventSystem::getInstance().notify(event);
}

void Player::increaseMaxHp(int amount) {
    int oldMaxHp = maxHp_;
    maxHp_ += amount;
    
    GameEvent event;
    event.type = EventType::UPGRADE_APPLIED;
    event.actor = "Player";
    event.details = "Макс. HP: " + std::to_string(oldMaxHp) + " -> " + std::to_string(maxHp_);
    EventSystem::getInstance().notify(event);
}

void Player::increaseRangedRange(int amount) {
    int oldRange = rangedRange_;
    rangedRange_ += amount;
    
    GameEvent event;
    event.type = EventType::UPGRADE_APPLIED;
    event.actor = "Player";
    event.details = "Дальность: " + std::to_string(oldRange) + " -> " + std::to_string(rangedRange_);
    EventSystem::getInstance().notify(event);
}

void Player::increaseSlowResistance(float amount) {
    float oldResistance = slowResistance_;
    slowResistance_ = std::min(1.0f, slowResistance_ + amount);
    
    GameEvent event;
    event.type = EventType::UPGRADE_APPLIED;
    event.actor = "Player";
    event.details = "Сопр. замедлению: " + std::to_string(oldResistance*100) + "% -> " + std::to_string(slowResistance_*100) + "%";
    EventSystem::getInstance().notify(event);
}