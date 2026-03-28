#include "Hand.h"
#include "Player.h"
#include "GameField.h"
#include <iostream>
#include <utility>

Hand::Hand(size_t maxSize) : m_maxSize(maxSize) {
}

Hand::Hand(Hand&& other) noexcept
    : m_spells(std::move(other.m_spells)),
    m_maxSize(other.m_maxSize) {
}

Hand& Hand::operator=(Hand&& other) noexcept {
    if (this != &other) {
        m_spells = std::move(other.m_spells);
        m_maxSize = other.m_maxSize;
    }
    return *this;
}

bool Hand::addSpell(std::unique_ptr<Spell> spell) {
    if (!spell) {
        std::cout << "Warning: Attempted to add null spell to hand" << std::endl;
        return false;
    }

    if (m_spells.size() >= m_maxSize) {
        return false;
    }
    m_spells.push_back(std::move(spell));
    return true;
}

void Hand::removeSpell(size_t index) {
    if (index < m_spells.size()) {
        m_spells.erase(m_spells.begin() + index);
    }
}

Spell* Hand::getSpell(size_t index) const {
    if (index >= m_spells.size()) {
        return nullptr;
    }

    // Дополнительная проверка на null
    if (!m_spells[index]) {
        std::cout << "Warning: Null spell found in hand at index " << index << std::endl;
        return nullptr;
    }

    return m_spells[index].get();
}

size_t Hand::getSpellCount() const {
    return m_spells.size();
}

size_t Hand::getMaxSize() const {
    return m_maxSize;
}

void Hand::displaySpells(const Player& player, const GameField& field) const {
    if (m_spells.empty()) {
        std::cout << "No spells in hand." << std::endl;
        return;
    }

    std::cout << "\n--- YOUR SPELLS ---" << std::endl;
    for (size_t i = 0; i < m_spells.size(); ++i) {
        if (m_spells[i]) {
            std::cout << i + 1 << ". " << m_spells[i]->getName()
                << " (Cost: " << m_spells[i]->getManaCost() << " mana)" << std::endl;
        }
    }
}