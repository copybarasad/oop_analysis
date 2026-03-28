#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include <vector>
#include <memory>

class SpellCard;
struct Position;

class SpellHand {
public:
    SpellHand(int maxSize);

    bool addSpell(std::shared_ptr<SpellCard> spell);

    bool useSpell(int index, const Position& casterPos, const Position& targetPos);

    std::shared_ptr<SpellCard> getSpell(int index) const;

    void removeSpell(int index);

    int getSpellCount() const;

    int getMaxSize() const;

    bool isFull() const;

    static std::shared_ptr<SpellCard> createRandomSpell();

private:
    std::vector<std::shared_ptr<SpellCard>> spells_;
    int maxSize_;
};

#endif