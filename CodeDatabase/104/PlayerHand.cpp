#include "PlayerHand.h"

// Конструктор - инициализирует пустую руку
PlayerHand::PlayerHand() {
}

// Добавление заклинания в руку
bool PlayerHand::addSpell(std::unique_ptr<SpellCard> spell) {
    if (spells_.size() >= MAX_SIZE) {
        return false;
    }
    spells_.push_back(std::move(spell));
    return true;
}

// Удаление заклинания по индексу
bool PlayerHand::removeSpell(size_t index) {
    if (index >= spells_.size()) {
        return false;
    }
    spells_.erase(spells_.begin() + index);
    return true;
}

// Получение заклинания по индексу
SpellCard* PlayerHand::getSpell(size_t index) const {
    if (index >= spells_.size()) {
        return nullptr;
    }
    return spells_[index].get();
}

// Количество заклинаний в руке
size_t PlayerHand::getSpellCount() const {
    return spells_.size();
}

// Максимальное количество заклинаний
size_t PlayerHand::getMaxSize() const {
    return MAX_SIZE;
}

// Проверка заполненности руки
bool PlayerHand::isFull() const {
    return spells_.size() >= MAX_SIZE;
}