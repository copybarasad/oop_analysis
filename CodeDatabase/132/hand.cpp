#include "hand.hpp"

Hand::Hand(size_t capacity) : capacity(capacity) {}
Hand::Hand(size_t capacity, std::shared_ptr<ISpell> initial_spell): capacity(capacity) {
    cards.push_back(initial_spell);
}

Hand::Hand(HandImage img):
    capacity(img.capacity),
    enhancement_level(img.enhancement_level)
{}

std::string Hand::get_spell_str() {
    std::string res = "";

    for (const auto& spell: cards) {
        res.push_back(spell->get_symbol());
    }

    return res;
}

HandImage Hand::get_data_image() {
    return HandImage(
        capacity,
        enhancement_level,
        get_spell_str()
    );
}

bool Hand::add_spell(char spell) {
    auto s = SpellFactory::create(spell);
    if (!s)
        return false;

    return add_spell(s);
}

bool Hand::add_spell(std::shared_ptr<ISpell> spell) {
    if (cards.size() >= capacity)
        return false;
    cards.push_back(spell);
    return true;
}

std::shared_ptr<ISpell> Hand::get_spell(size_t index) {
    if (index >= cards.size())
        return nullptr;

    auto spell = cards[index];

    cards.erase(cards.begin() + index);

    enhancement_level = spell->apply_enhancement(enhancement_level);

    return spell;
}