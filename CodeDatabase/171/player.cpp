#include "player.h"
#include <iostream>

Player::Player(int maxHealth, int damage, Position startPosition, int handSize)
    : maxHealth_(maxHealth),
    health_(maxHealth),
    meleeDamage_(damage),
    rangedDamage_(std::max(1, damage / 2)), // √арантируем минимум 1 урон
    position_(startPosition),
    fightMode_(true),
    spellHand_(std::make_unique<SpellHand>(handSize)),
    killCount_(0) {

    // ƒобавл€ем начальное случайное заклинание
    auto startingSpell = SpellHand::createRandomSpell();
    spellHand_->addSpell(startingSpell);
}

void Player::takeDamage(int damage) {
    health_ -= damage;
    if (health_ < 0) health_ = 0;
}

void Player::switchCombatMode() {
    fightMode_ = !fightMode_;
}

void Player::move(Position newPosition) {
    position_ = newPosition;
}

bool Player::castSpell(int spellIndex, Position targetPos) {
    return spellHand_->useSpell(spellIndex, position_, targetPos);
}

void Player::addSpell(std::shared_ptr<SpellCard> spell) {
    spellHand_->addSpell(spell);
}

SpellHand& Player::getSpellHand() {
    return *spellHand_;
}

int Player::getHealth() const {
    return health_;
}

Position Player::getPosition() const {
    return position_;
}

bool Player::isMeleeMode() const {
    return fightMode_;
}

int Player::getDamage() const {
    return fightMode_ ? meleeDamage_ : rangedDamage_;
}

bool Player::isAlive() const {
    return health_ > 0;
}

void Player::addKill() {
    killCount_++;

    //  аждые KILLS_PER_SPELL убийств получаем новое заклинание
    if (killCount_ % KILLS_PER_SPELL == 0 && !spellHand_->isFull()) {
        auto newSpell = SpellHand::createRandomSpell();
        if (spellHand_->addSpell(newSpell)) {
            std::cout << "ѕолучено новое заклинание за " << killCount_ << " убийств!" << std::endl;
        }
    }
}

int Player::getKillCount() const {
    return killCount_;
}

int Player::getKillsForNextSpell() const {
    return KILLS_PER_SPELL - (killCount_ % KILLS_PER_SPELL);
}