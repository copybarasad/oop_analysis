#include <algorithm>
#include <random>
#include <stdexcept>

#include "../map/map.hpp"
#include "direct-damage-spell.hpp"
#include "hand.hpp"

Hand::Hand(size_t capacity) {
    if (capacity <= 0) {
        throw std::invalid_argument("size must be positive");
    }

    m_Capacity = capacity;
    m_Upgrades = 0;
    m_Spells.push_back(new DirectDamageSpell(10, 5));
}

void Hand::addSpell(Spell *spell) {
    if (m_Spells.size() >= m_Capacity) {
        throw std::length_error("hand is full");
    }

    m_Spells.push_back(spell);
}

void Hand::addUpgrades(int upgrades) { m_Upgrades += upgrades; }

bool Hand::canCast(int index, Positionable *caster, Map *map, Field *target) {
    if (index < 0 || index >= m_Spells.size()) {
        return false;
    }

    return m_Spells[index]->canCast(caster, map, target);
}

void Hand::cast(int index, Positionable *caster, Map *map, Field *target) {
    if (index < 0 || index >= m_Spells.size()) {
        throw std::range_error("spell index out of range");
    }

    Spell *spell = m_Spells[index];

    while (m_Upgrades > 0) {
        spell->upgrade();
        m_Upgrades--;
    }

    spell->cast(this, caster, map, target);
    m_Spells.erase(m_Spells.begin() + index);
}

size_t Hand::len() { return m_Spells.size(); }
size_t Hand::cap() { return m_Capacity; }

void Hand::removeHalf() {
    size_t currentSize = m_Spells.size();
    size_t removeCount = currentSize / 2;

    if (removeCount <= 1) {
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(m_Spells.begin(), m_Spells.end(), gen);

    m_Spells.erase(m_Spells.begin(), m_Spells.begin() + removeCount);
}

bytedump Hand::dump() {
    bytedump bytes;
    // len + capacity + upgrades + (4 param bytes) * len.
    bytes.reserve(3 + 4 * len());

    bytes.push_back(static_cast<std::byte>(len()));
    bytes.push_back(static_cast<std::byte>(m_Capacity));
    bytes.push_back(static_cast<std::byte>(m_Upgrades));

    for (Spell *i : m_Spells) {
        bytedump spellBytes = i->dump();
        bytes.insert(bytes.end(), spellBytes.begin(), spellBytes.end());
    }

    return bytes;
}

Hand::Hand(bytedump bytes) {
    if (bytes.size() < 3) {
        throw std::length_error("hand dump must contain at least 3 bytes");
    }

    int length = static_cast<int>(bytes[0]);
    m_Capacity = static_cast<int>(bytes[1]);
    m_Upgrades = static_cast<int>(bytes[2]);

    if (bytes.size() != 3 + 4 * length) {
        throw std::length_error("wrong number of bytes in hand dump");
    }

    for (int i = 0; i < length; i++) {
        bytedump spellDump =
            bytedump(bytes.begin() + 3 + 4 * i, bytes.begin() + 3 + 4 * i + 4);

        m_Spells.push_back(Spell::fromByteDump(spellDump));
    }
}

std::string Hand::toString() {
    std::string res;

    for (Spell *s : m_Spells) {
        res += s->toString();
        res += '\n';
    }

    return res;
}
