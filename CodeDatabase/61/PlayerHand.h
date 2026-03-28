#ifndef PLAYERHAND_H
#define PLAYERHAND_H

#include "Spell.h"
#include <vector>
#include <memory>
#include <random>
#include <string>

class PlayerHand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    int maxSize;
    mutable std::mt19937 randomGenerator;
    
    // Фабричный метод для создания случайного заклинания
    std::unique_ptr<Spell> createRandomSpell() const;

public:
    PlayerHand(int handSize = 5);
    
    // Управление заклинаниями
    bool addSpell(std::unique_ptr<Spell> spell);
    bool removeSpell(size_t index);
    Spell* getSpell(size_t index) const;
    size_t getSpellCount() const;
    int getMaxSize() const;
    bool isFull() const;
    
    // Получение нового случайного заклинания
    void addRandomSpell();
    
    // Отображение руки
    void displayHand() const;
    
    // Применение улучшений к следующему заклинанию
    void applyPendingEnhancementToNext();
    
    // Serialization
    std::string serialize() const;
    bool deserialize(const std::string& data);
};

#endif // PLAYERHAND_H

