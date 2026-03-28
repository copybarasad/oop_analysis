#include "Player.h"

Player::Player(const std::string& playerName, int startX, int startY, int health, int mdamage, int rdamage)
    : Entity(playerName, startX, startY, health),
        score(0), meleeDamage(mdamage), rangedDamage(rdamage), damageMode(DamageMode::Melee), hand() {}

void Player::addScore(int sc) {
    score += sc;
}

PlayerHand Player::getHand() const {
    return hand;
}

void Player::addSpell(CommonSpell spell) {
    hand.addSpell(spell);
}

void Player::toggleDamageMode() {
    damageMode = damageMode == DamageMode::Melee ? DamageMode::Ranged: DamageMode::Melee;
}
