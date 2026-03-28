#include "Player.h"

Player::Player(int maxHealth, int nearDamage, int farDamage)
        : Entity(maxHealth, 0, "Player"), hand_(5),
          hpMax_(maxHealth), isNear_(true),
          nearDamage_(nearDamage), farDamage_(farDamage), score_(0),
          skipNextMove_(false)
{
}

std::shared_ptr<Entity> Player::clone() const {
    return std::make_shared<Player>(*this);
}

Hand& Player::hand() { return hand_; }
const Hand& Player::hand() const { return hand_; }

int Player::hpMax() const noexcept { return hpMax_; }

void Player::switchMode() { isNear_ = !isNear_; }

int Player::getDamageForCurrentMode() const noexcept {
    return isNear_ ? nearDamage_ : farDamage_;
}

void Player::takeDamage(int d) {
    Entity::takeDamage(d);
}

void Player::addScore(int s) { score_ += s; }
int Player::score() const noexcept { return score_; }

void Player::accumulateUpgrade(int r, int w, int h, int trapD, int spawnN) {
    // kept as placeholder (Hand handles accumulation in your original code)
}

void Player::clearAccumulation() {
}

bool Player::isNear() const noexcept { return isNear_; }
std::string Player::modeName() const { return isNear_ ? "Near" : "Far"; }

void Player::setSkipNextMove(bool v) { skipNextMove_ = v; }
bool Player::consumeSkipMove() { bool prev = skipNextMove_; skipNextMove_ = false; return prev; }
bool Player::hasSkipNextMove() const noexcept { return skipNextMove_; }

void Player::increaseHpMax(int add) {
    if (add <= 0) return;
    hpMax_ += add;
    restoreToFullHp();
}

void Player::increaseNearDamage(int add) {
    if (add <= 0) return;
    nearDamage_ += add;
}

void Player::increaseFarDamage(int add) {
    if (add <= 0) return;
    farDamage_ += add;
}

void Player::setHandCapacity(size_t newCap) {
    if (newCap == 0) return;
    Hand newHand(newCap);
    const auto &spells = hand_.spells();
    for (const auto &s : spells) {
        if (newHand.size() < newHand.capacity()) newHand.addSpell(s);
    }
    hand_ = std::move(newHand);
}

void Player::increaseHandCapacity(size_t add) {
    if (add == 0) return;
    setHandCapacity(hand_.capacity() + add);
}

void Player::restoreToFullHp() {
    hp_ = hpMax_;
}

void Player::setHp(int hp) {
    if (hp < 0) hp = 0;
    if (hp > hpMax_) hp = hpMax_;
    hp_ = hp;
}



