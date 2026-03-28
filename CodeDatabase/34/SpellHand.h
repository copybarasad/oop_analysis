#ifndef GAME_SPELL_HAND_H
#define GAME_SPELL_HAND_H

#include <vector>
#include <memory>
#include <iostream>
#include "spells/ISpell.h"

namespace Game {

class SpellHand {
public:
    explicit SpellHand(size_t maxSize);

    // Методы, изменяющие состояние
    bool addSpell(std::unique_ptr<ISpell> spell);
    std::unique_ptr<ISpell> useSpell(size_t index);
    void removeRandomSpells(size_t count);

    // Методы для чтения
    ISpell* getSpell(size_t index);
    const ISpell* getSpell(size_t index) const;
    
    const std::vector<std::unique_ptr<ISpell>>& getAllSpells() const;
    size_t getSpellCount() const;
    size_t getMaxSize() const;

    // Методы для сериализации
    void serialize(std::ostream& os) const;
    void deserialize(std::istream& is);

    static std::unique_ptr<ISpell> createRandomSpell();

private:
    std::vector<std::unique_ptr<ISpell>> spells;
    size_t maxSize;
};

}

#endif