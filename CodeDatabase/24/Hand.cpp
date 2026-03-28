#include "Hand.hpp"
#include "SpellDirectDamage.hpp"
#include "SpellAreaDamage.hpp"
#include "Configs.hpp"
#include <random>
#include <iostream>

Hand::Hand(size_t maxSize)
    : maxSize(maxSize) {
    // Проверяем валидность размера руки согласно Config
    if (!Config::Constraints::validateHandSize(static_cast<int>(maxSize))) {
        throw std::invalid_argument("Invalid hand size");
    }
}

// Конструктор копирования
Hand::Hand(const Hand& other)
    : maxSize(other.maxSize) {
    spells.reserve(other.spells.size());
    for (const auto& s : other.spells) {
        if (s) spells.push_back(s->clone());
    }
}

// Оператор присваивания копированием
Hand& Hand::operator=(const Hand& other) {
    if (this == &other) return *this;
    maxSize = other.maxSize;
    spells.clear();
    spells.reserve(other.spells.size());
    for (const auto& s : other.spells) {
        if (s) spells.push_back(s->clone());
    }
    return *this;
}

bool Hand::addSpell(std::unique_ptr<ISpell> spell) {
    if (spells.size() >= maxSize) return false;
    spells.push_back(std::move(spell));
    return true;
}

bool Hand::useSpell(size_t index, Field& field, const Pos& casterPos, const Pos& target) {
    if (index >= spells.size()) return false;
    ISpell* s = spells[index].get();
    if (!s) return false;

    bool used = s->cast(field, casterPos, target);
    if (used) {
        // удаляем использованную карту
        spells.erase(spells.begin() + static_cast<std::ptrdiff_t>(index));
    }
    return used;
}

size_t Hand::size() const { return spells.size(); }
size_t Hand::capacity() const { return maxSize; }

const ISpell* Hand::getSpell(size_t index) const {
    if (index >= spells.size()) return nullptr;
    return spells[index].get();
}

void Hand::addRandomSpell() {
    // Простая рандомизация: 50/50
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1);

    if (dist(gen) == 0) {
        addSpell(std::make_unique<SpellDirectDamage>(
            Config::Settings::SPELL_DIRECT_DAMAGE,
            Config::Settings::SPELL_DIRECT_RANGE));
        std::cout << "Received spell: Direct Damage" << std::endl;
    } else {
        addSpell(std::make_unique<SpellAreaDamage>(
            Config::Settings::SPELL_AREA_DAMAGE,
            Config::Settings::SPELL_AREA_RANGE));
        std::cout << "Received spell: Area Damage (2x2)" << std::endl;
    }
}

bool Hand::removeSpell(size_t index) {
    if (index >= spells.size()) return false;
    spells.erase(spells.begin() + static_cast<std::ptrdiff_t>(index));
    return true;
}
