#include "hand.hpp"
#include "spell_pool.hpp"

Hand::Hand(std::size_t cap) : cap_(cap), slots_(cap_) {}

std::size_t Hand::size() const noexcept {
    std::size_t cnt = 0;
    for (auto const& p : slots_) if (p) ++cnt;
    return cnt;
}

bool Hand::has(std::size_t i) const noexcept {
    return i < cap_ && static_cast<bool>(slots_[i]);
}

Spell* Hand::get(std::size_t i) noexcept {
    if (i >= cap_) return nullptr;
    return slots_[i].get();
}

const Spell* Hand::get(std::size_t i) const noexcept {
    if (i >= cap_) return nullptr;
    return slots_[i].get();
}

void Hand::remove(std::size_t i) noexcept {
    if (i >= cap_) return;
    slots_[i].reset();   // очищаем только выбранный слот
}

std::string Hand::cardName(std::size_t i) const {
    if (!has(i)) return "(пусто)";
    return slots_[i]->name();
}

bool Hand::drawRandom() {
    std::size_t idx = cap_;
    for (std::size_t i = 0; i < cap_; ++i) {
        if (!slots_[i]) { idx = i; break; }
    }
    if (idx == cap_) return false;

    slots_[idx] = makeRandomSpell();
    return static_cast<bool>(slots_[idx]);
}
