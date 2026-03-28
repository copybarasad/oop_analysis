#ifndef HAND_H
#define HAND_H

#include <vector>
#include <memory>
#include "ISpell.h"

class Hand {
public:
    Hand(size_t capacity);
    void addSpell(std::unique_ptr<ISpell> spell);
    void addRandomSpell();
    bool useSpell(size_t index, Player& player, GameField& field, int tx, int ty);
    size_t size() const;
    size_t capacity() const;
    void list() const;
    std::vector<std::unique_ptr<ISpell>> extractAllCards();
    std::vector<std::string> getSpellNames() const;
private:
    std::vector<std::unique_ptr<ISpell>> cards_;
    size_t capacity_;
};
#endif // HAND_H
