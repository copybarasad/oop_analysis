#include "Hand.h"
#include <algorithm>
#include <iostream>
#include <cstdlib>

Hand::Hand() : m_selectedIndex(0) {}

Hand::~Hand() { clear(); }

void Hand::clear() {
    for (auto* s : m_spells) delete s;
    m_spells.clear();
    m_selectedIndex = 0;
}

void Hand::addSpell(Spell* spell) {
    if (!isFull()) {
        m_spells.push_back(spell);
    } else {
        delete spell; 
    }
}

Spell* Hand::findSpell(SpellType type) {
    for(auto* s : m_spells) {
        if(s->getType() == type) return s;
    }
    return nullptr;
}

const Spell* Hand::getSpell(int index) const {
    if (index < 0 || index >= (int)m_spells.size()) return nullptr;
    return m_spells[index];
}

Spell* Hand::getSpellMutable(int index) {
    if (index < 0 || index >= (int)m_spells.size()) return nullptr;
    return m_spells[index];
}

int Hand::getSize() const { return m_spells.size(); }

void Hand::nextSpell() {
    if (m_spells.empty()) return;
    m_selectedIndex = (m_selectedIndex + 1) % m_spells.size();
}

void Hand::prevSpell() {
    if (m_spells.empty()) return;
    m_selectedIndex = (m_selectedIndex - 1 + m_spells.size()) % m_spells.size();
}

int Hand::getSelectedIndex() const { return m_selectedIndex; }

void Hand::removeRandomSpell() {
    if (m_spells.empty()) return;
    int idx = rand() % m_spells.size();
    delete m_spells[idx];
    m_spells.erase(m_spells.begin() + idx);
    if (m_selectedIndex >= (int)m_spells.size()) m_selectedIndex = 0;
}

bool Hand::isFull() const {
    return m_spells.size() >= MAX_HAND_SIZE;
}

bool Hand::upgradeRandomSpell() {
    if (m_spells.empty()) return false;
    std::vector<Spell*> upgradeable;
    for(auto* s : m_spells) {
        if (s->canUpgrade()) upgradeable.push_back(s);
    }
    if (upgradeable.empty()) return false;
    int idx = rand() % upgradeable.size();
    upgradeable[idx]->upgrade();
    return true;
}
