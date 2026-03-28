#pragma once
#include <vector>
#include <memory>
#include <cstddef>
#include <string>
#include "ISpell.hpp"

class Field;
class Pos;

// "Рука" игрока — содержит заклинания. Простая реализация:
// Использование карты по индексу (удаляется после применения)
class Hand {
private:
    size_t maxSize;
    std::vector<std::unique_ptr<ISpell>> spells;

public:
    explicit Hand(size_t maxSize = 3);

    Hand(const Hand& other);

    Hand& operator=(const Hand& other);

    // Перемещающие операции по умолчанию
    Hand(Hand&&) noexcept = default;
    Hand& operator=(Hand&&) noexcept = default;

    bool addSpell(std::unique_ptr<ISpell> spell);
    bool useSpell(size_t index, Field& field, const Pos& casterPos, const Pos& target);

    size_t size() const;
    size_t capacity() const;

    const ISpell* getSpell(size_t index) const;

    // Добавить одино случайное заклинание
    void addRandomSpell();

    bool removeSpell(size_t index);
};
