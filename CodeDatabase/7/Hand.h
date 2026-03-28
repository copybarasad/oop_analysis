#ifndef HAND_H
#define HAND_H

#include <vector>
#include <memory>
#include <iostream>
#include "Spell.h"

class Player;
class Enemy;
class Field;

class Hand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    const int maxSize;

public:
    Hand(int maxSize);
    ~Hand();
    
    // Основные методы
    void addSpell(std::unique_ptr<Spell> spell);
    void useSpell(int index, Player& caster, std::vector<Enemy>& enemies, Field& field);
    size_t getSpellCount() const;
    void printHand() const;  // Добавили const
    void removeRandomSpells();
    
    // Метод для получения названия заклинания
    std::string getSpellName(int index) const {
        if (index >= 0 && index < spells.size()) {
            return spells[index]->getName();
        }
        return "";
    }
    
    // Сохранение/загрузка
    void save(std::ostream& os) const;
    void load(std::istream& is);
    
    // Геттеры
    bool isFull() const { return spells.size() >= maxSize; }
    bool isEmpty() const { return spells.empty(); }
    int getMaxSize() const { return maxSize; }
    
    // Для доступа к заклинаниям
    Spell* getSpell(int index) {
        if (index < 0 || index >= spells.size()) return nullptr;
        return spells[index].get();
    }
    
    const Spell* getSpell(int index) const {
        if (index < 0 || index >= spells.size()) return nullptr;
        return spells[index].get();
    }
    
    const std::vector<std::unique_ptr<Spell>>& getAllSpells() const { return spells; }
};

#endif // HAND_H