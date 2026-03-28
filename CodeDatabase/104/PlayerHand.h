#ifndef PLAYERHAND_H
#define PLAYERHAND_H

#include <vector>
#include <memory>
#include "SpellCard.h"

// Рука игрока - хранит заклинания
class PlayerHand {
private:
    std::vector<std::unique_ptr<SpellCard>> spells_;
    static const size_t MAX_SIZE = 4; // Максимум 4 заклинания

public:
    PlayerHand();
    bool addSpell(std::unique_ptr<SpellCard> spell);
    bool removeSpell(size_t index);
    SpellCard* getSpell(size_t index) const;
    size_t getSpellCount() const;
    size_t getMaxSize() const;
    bool isFull() const;
};

#endif