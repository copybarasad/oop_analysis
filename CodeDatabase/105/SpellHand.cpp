#include "SpellHand.h"
#include <algorithm>
#include <iostream>

// Конструктор
SpellHand::SpellHand(size_t maxSize) : maxSize(maxSize) {}

// Добавление заклинания в руку
bool SpellHand::addSpell(std::unique_ptr<Spell> spell) {
    if (isFull() || !spell) {
        return false;
    }
    spells.push_back(std::move(spell));
    return true;
}

// Удаление заклинания по индексу
bool SpellHand::removeSpell(size_t index) {
    if (index >= spells.size()) {
        return false;
    }
    spells.erase(spells.begin() + index);
    return true;
}

// Получение заклинания по индексу (без передачи владения)
Spell* SpellHand::getSpell(size_t index) const {
    if (index >= spells.size()) {
        return nullptr;
    }
    return spells[index].get();
}

// Получение количества заклинаний в руке
size_t SpellHand::getSpellCount() const {
    return spells.size();
}

// Получение максимального размера руки
size_t SpellHand::getMaxSize() const {
    return maxSize;
}

// Проверка, заполнена ли рука
bool SpellHand::isFull() const {
    return spells.size() >= maxSize;
}

// Очистка всех заклинаний из руки
void SpellHand::clear() {
    spells.clear();
}