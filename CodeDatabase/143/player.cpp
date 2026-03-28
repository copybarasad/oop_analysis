#include "player.h"
#include <fstream>

Player::Player()
    : GameObject(Position(0, 0),
        GameConstants::INITIAL_PLAYER_HEALTH,
        GameConstants::INITIAL_PLAYER_DAMAGE),
    score(0),
    hand(3) {
}

void Player::move(int deltaX, int deltaY) {
    currentPosition.setX(currentPosition.getX() + deltaX);
    currentPosition.setY(currentPosition.getY() + deltaY);
}

int Player::getScore() const {
    return score;
}

void Player::addScore(int points) {
    score += points;
}

void Player::setScore(int newScore) {
    score = newScore;
}

bool Player::learnSpell(std::unique_ptr<SpellCard> spell) {
    return hand.addSpell(std::move(spell));
}

bool Player::canLearnSpell() const {
    return hand.canAddSpell();
}

const PlayerHand& Player::getHand() const {
    return hand;
}

bool Player::castSpell(int spellIndex) {
    auto spell = hand.getSpell(spellIndex);
    if (!spell) {
        return false;
    }

    return spell->cast();
}

bool Player::canAffordSpell(int cost) const {
    return score >= cost;
}

bool Player::buySpell(std::unique_ptr<SpellCard> spell, int cost) {
    if (!canAffordSpell(cost) || !canLearnSpell()) {
        return false;
    }

    score -= cost;
    return learnSpell(std::move(spell));
}

bool Player::removeSpell(int index) {
    return hand.removeSpell(index);
}

void Player::save(std::ostream& out) const {
    GameObject::save(out);
    out.write(reinterpret_cast<const char*>(&score), sizeof(score));
    hand.save(out);
}

void Player::load(std::istream& in) {
    GameObject::load(in);
    in.read(reinterpret_cast<char*>(&score), sizeof(score));
    hand.load(in);
}