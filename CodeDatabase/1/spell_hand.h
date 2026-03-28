#pragma once
#include "spell.h"
#include <vector>

class SpellHand {
private:
    std::vector<Spell*> spells_;
    int maxSlots_;
    std::vector<SpellType> obtainedSpellsHistory_;  //

    void clearSpells();
    Spell* createSpellCopy(Spell* original) const;

public:
    SpellHand(int maxSlots);
    ~SpellHand();
    // Конструкторы/операторы
    SpellHand(const SpellHand& other);
    SpellHand(SpellHand&& other) noexcept;
    SpellHand& operator=(const SpellHand& other);
    SpellHand& operator=(SpellHand&& other) noexcept;
    
    // Основной интерфейс
    bool tryAddSpell(Spell* spell);//
    bool addSpell(Spell* spell);  // Простое добавление без проверок
    bool removeSpell(int index);
    Spell* getSpell(int index) const;
    
    // Проверки
    bool isFull() const;
    bool wasSpellEverObtained(SpellType spell) const; //
    
    // Информация
    int getSpellCount() const;
    int getMaxSlots() const;
    void addRandomStarterSpell();
};