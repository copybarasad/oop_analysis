#ifndef ISPELL_H
#define ISPELL_H

#include "SpellType.hpp"
#include <memory>

class GameState;
class SpellCaster;

class ISpell {
public:
    virtual ~ISpell() = default;
    
    virtual bool cast(int targetX, int targetY, GameState& gameState, SpellCaster& caster) = 0;
    virtual SpellType getType() const = 0;
    virtual int getManaCost() const = 0;
    virtual std::unique_ptr<ISpell> clone() const = 0;
    virtual void applyEnhancements(int enhancementCount) = 0;

    virtual void serialize(std::ostream& os) const = 0;
    virtual void deserialize(std::istream& is) = 0;
};

#endif