#pragma once
#include "Coords.h"
#include <vector>
#include <string>


class Hand;
class SpellCardVisitor;
class GameArea;

struct SpellContext {
    Coords caster_position;
    std::vector<std::string> target_types;
    Hand* caster_hand;

    SpellContext(Coords pos, std::vector<std::string> targets, Hand* hand)
        : caster_position(pos), target_types(std::move(targets)), caster_hand(hand) {
    }
};

class SpellCard {
public:
    virtual ~SpellCard() = default;

    virtual bool apply(const SpellContext& context, GameArea& gamearea) = 0;
    virtual std::string accept(const SpellCardVisitor& visitor) const = 0;
};