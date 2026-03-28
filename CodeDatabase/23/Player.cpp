#include "Player.h"
#include "PlayerHand.h"

Player::Player(int x, int y, int hp, int base_dmg) 
    : Entity(x, y, hp), base_dmg(base_dmg), attack_type(AttackType::MELEE), 
      score(0), is_slowed(false), slowed_next_turn(false), hand(new PlayerHand(PLAYER_HAND_CAPACITY)) {
    hand->addRandomSpell();
}

int Player::getBaseDamage() const {
    return base_dmg;
}

AttackType Player::getAttackType() const {
    return attack_type;
}

int Player::getScore() const {
    return score;
}

bool Player::getIsSlowed() const {
    return is_slowed;
}

int Player::getCurrentDamage() const {
    return base_dmg + (attack_type == AttackType::RANGE ? RANGE_ATTACK_BONUS : MELEE_ATTACK_BONUS);
}

void Player::switchAttackType() {
    attack_type = (attack_type == AttackType::RANGE) ? AttackType::MELEE : AttackType::RANGE;
}

void Player::addScore(int points) {
    score += points;
    if (score % SCORE_MILESTONE_FOR_SPELL == 0 && hand) {
        hand->addRandomSpell();
    }
}

void Player::setSlowed(bool slowed) {
    is_slowed = slowed;
}

void Player::setSlowedNextTurn(bool slowed) {
    slowed_next_turn = slowed;
}

bool Player::getSlowedNextTurn() const {
    return slowed_next_turn;
}

void Player::setScore(int newScore) {
    score = newScore < 0 ? 0 : newScore;
}
