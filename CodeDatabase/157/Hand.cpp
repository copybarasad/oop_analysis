#include "Hand.h"
#include <iostream>
#include <cstdlib>
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"

Hand::Hand(size_t capacity) : capacity_(capacity) {}

void Hand::addSpell(std::unique_ptr<ISpell> spell) {
    if (cards_.size() < capacity_) {
        cards_.push_back(std::move(spell));
    } else {
        std::cout << "Hand: рука полна, карта не добавлена.\n";
    }
}

void Hand::addRandomSpell() {
    int r = std::rand() % 3;
    if (r == 0) addSpell(std::make_unique<DirectDamageSpell>(5, 3));
    else if (r == 1) addSpell(std::make_unique<AreaDamageSpell>(3, 3));
    else addSpell(std::make_unique<TrapSpell>(4, 3));
}

bool Hand::useSpell(size_t index, Player& player, GameField& field, int tx, int ty) {
    if (index >= cards_.size()) {
        std::cout << "Hand: неверный индекс.\n";
        return false;
    }
    bool ok = cards_[index]->use(player, field, tx, ty);
    if (ok) {
        cards_.erase(cards_.begin() + index);
    }
    return ok;
}

size_t Hand::size() const { return cards_.size(); }
size_t Hand::capacity() const { return capacity_; }

void Hand::list() const {
    std::cout << "Hand (" << cards_.size() << "/" << capacity_ << "):\n";
    for (size_t i = 0; i < cards_.size(); ++i) {
        std::cout << i << ": " << cards_[i]->name() << "\n";
    }
}

std::vector<std::unique_ptr<ISpell>> Hand::extractAllCards() {
    std::vector<std::unique_ptr<ISpell>> out = std::move(cards_);
    cards_.clear();
    return out;
}

std::vector<std::string> Hand::getSpellNames() const {
    std::vector<std::string> names;
    for (const auto& c : cards_) {
        names.push_back(c->name());
    }
    return names;
}
