#ifndef SPELLHAND_H
#define SPELLHAND_H

#include "Spell.h"
#include <memory>
#include <deque>

/**
 * @file SpellHand.h
 * @brief Класс руки заклинаний игрока
 */

class Game;

/**
 * @class SpellHand
 * @brief Управляет коллекцией заклинаний игрока
 * 
 * Features:
 * - Ограниченный размер руки
 * - Система Boost (усиление следующего заклинания)
 * - Генерация случайных заклинаний
 */
class SpellHand {
private:
    std::deque<std::unique_ptr<Spell>> spells;  ///< Заклинания в руке
    int maxHandSize;  ///< Максимальный размер руки
    int boostStacks;  ///< Количество активных стеков Boost
    
public:
    /**
     * @brief Конструктор
     * @param maxSize Максимальное количество заклинаний в руке
     */
    explicit SpellHand(int maxSize = 5);
    
    /**
     * @brief Добавить заклинание в руку
     * @param spell Умный указатель на заклинание
     * @return true если успешно добавлено
     */
    bool addSpell(std::unique_ptr<Spell> spell);
    
    /**
     * @brief Использовать заклинание без координат (Boost, Heal)
     * @param index Индекс заклинания в руке (0-based)
     * @param game Указатель на игру
     * @return true если заклинание успешно использовано
     */
    bool useSpell(int index, Game* game);
    
    /**
     * @brief Использовать заклинание с координатами (Area Damage, Trap, Summon)
     * @param index Индекс заклинания в руке (0-based)
     * @param game Указатель на игру
     * @param targetX Координата X цели
     * @param targetY Координата Y цели
     * @return true если заклинание успешно использовано
     */
    bool useSpell(int index, Game* game, int targetX, int targetY);
    
    /**
     * @brief Добавить случайное заклинание
     * 
     * Генерирует одно из 5 типов заклинаний случайным образом.
     */
    void addRandomSpell();
    
    /**
     * @brief Удалить заклинание по индексу
     * @param index Индекс заклинания
     * @return true если заклинание удалено
     */
    bool removeSpell(int index) {
        if (index < 0 || index >= static_cast<int>(spells.size())) {
            return false;
        }
        spells.erase(spells.begin() + index);
        return true;
    }
    
    /**
     * @brief Отобразить содержимое руки (debug)
     */
    void displayHand() const;
    
    /**
     * @brief Добавить стек усиления
     */
    void addBoostStack() {
        boostStacks++;
    }
    
    /**
     * @brief Сбросить стеки усиления
     */
    void resetBoostStacks() {
        boostStacks = 0;
    }
    
    /**
     * @brief Увеличить максимальный размер руки
     * @param amount Количество слотов для добавления
     */
    void increaseMaxSize(int amount) {
        maxHandSize += amount;
    }
    
    // Геттеры
    
    /**
     * @brief Получить количество заклинаний в руке
     * @return Количество заклинаний
     */
    int getSpellCount() const {
        return static_cast<int>(spells.size());
    }
    
    /**
     * @brief Получить максимальный размер руки
     * @return Максимальное количество заклинаний
     */
    int getMaxHandSize() const {
        return maxHandSize;
    }
    
    /**
     * @brief Получить количество активных стеков Boost
     * @return Количество стеков
     */
    int getBoostStacks() const {
        return boostStacks;
    }
    
    /**
     * @brief Проверить полная ли рука
     * @return true если в руке максимальное количество заклинаний
     */
    bool isFull() const {
        return spells.size() >= static_cast<size_t>(maxHandSize);
    }
    
    /**
     * @brief Проверить пустая ли рука
     * @return true если в руке нет заклинаний
     */
    bool isEmpty() const {
        return spells.empty();
    }
    
    /**
     * @brief Получить заклинание по индексу (константная версия)
     * @param index Индекс заклинания
     * @return Указатель на заклинание или nullptr
     */
    const Spell* getSpell(int index) const {
        if (index < 0 || index >= static_cast<int>(spells.size())) {
            return nullptr;
        }
        return spells[index].get();
    }
    
    /**
     * @brief Получить заклинание по индексу (неконстантная версия)
     * @param index Индекс заклинания
     * @return Указатель на заклинание или nullptr
     */
    Spell* getSpellAt(int index) {
        if (index < 0 || index >= static_cast<int>(spells.size())) {
            return nullptr;
        }
        return spells[index].get();
    }
    
    /**
     * @brief Получить константную ссылку на вектор заклинаний
     * @return Константная ссылка на deque заклинаний
     */
    const std::deque<std::unique_ptr<Spell>>& getSpells() const {
        return spells;
    }
};

#endif // SPELLHAND_H


