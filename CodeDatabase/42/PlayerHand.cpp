#include "PlayerHand.h"
#include <stdexcept>
#include <utility>

PlayerHand::PlayerHand(int maxHandSize)
    : maxSize(maxHandSize) {
    if (maxHandSize <= 0) {
        throw std::invalid_argument("Hand size must be positive");
    }
}

PlayerHand::PlayerHand(PlayerHand&& other) noexcept
    : hand(std::move(other.hand))
    , maxSize(other.maxSize) {
}

PlayerHand& PlayerHand::operator=(PlayerHand&& other) noexcept {
    if (this != &other) {
        hand = std::move(other.hand);
        maxSize = other.maxSize;
    }
    return *this;
}

bool PlayerHand::addSpell(std::unique_ptr<Spell> spell) {
    if (!spell) {
        throw std::invalid_argument("Spell pointer cannot be null");
    }
    if (isHandFull()) {
        return false;
    }
    hand.emplace_back(std::move(spell));
    return true;
}

bool PlayerHand::removeSpell(int index) {
    if (index < 0 || index >= static_cast<int>(hand.size())) {
        return false;
    }
    hand.erase(hand.begin() + index);
    return true;
}

SpellCard* PlayerHand::getSpell(int index) {
    if (index < 0 || index >= static_cast<int>(hand.size())) {
        return nullptr;
    }
    return &hand[index];
}

int PlayerHand::getHandSize() const {
    return hand.size();
}

int PlayerHand::getMaxSize() const {
    return maxSize;
}

bool PlayerHand::isHandFull() const {
    return hand.size() >= static_cast<size_t>(maxSize);
}

std::vector<SpellCard>& PlayerHand::getHand() {
    return hand;
}

const std::vector<SpellCard>& PlayerHand::getHand() const {
    return hand;
}