#ifndef HAND_H
#define HAND_H

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include "i_spell.h"
#include "basespell.h"
#include "spell_factory.h"

class Hand {
private:
    std::vector<std::unique_ptr<ISpell>> _spells;
    size_t _maxSize;

public:
    explicit Hand(size_t maxSize);

    // Добавить случайное заклинание (если есть место)
    void addRandomSpell();

    // Добавить конкретное заклинание (если есть место)
    void addSpell(std::unique_ptr<ISpell> spell);

    // Очистить использованные заклинания
    void removeUsedSpells();

    void clear() { _spells.clear(); }
    // Добавляем конструктор копирования и оператор присваивания
    Hand(const Hand& other);
    Hand& operator=(const Hand& other);
    
    // Перемещение
    Hand(Hand&&) = default;
    Hand& operator=(Hand&&) = default;
    
    // Получить количество заклинаний
    size_t size() const { return _spells.size(); }

    // Доступ к заклинанию по индексу
    ISpell* getSpell(size_t index) const;

    // Вывести список заклинаний в руке
    void printHand() const;
};

#endif // HAND_H
