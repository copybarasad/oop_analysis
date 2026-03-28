#ifndef SPELL_H
#define SPELL_H

#include "SpellTypes.h"
#include "Position.h"
#include <string>
#include <memory>

class IGameWorldFacade;

struct SpellContext {
    IGameWorldFacade* world;
    Pos targetPosition;
    
    SpellContext(IGameWorldFacade* w, Pos target)
        : world(w), targetPosition(target) {}
};

class Spell {
public:
    virtual ~Spell() = default;

    virtual bool cast(SpellContext& context, EnhancementState& enhancements) = 0;
    
    virtual void applyPermanentBonus(const EnhancementState& bonus) = 0;

    virtual std::string getName() const = 0;

    virtual std::string getDescription() const = 0;

    virtual SpellType getType() const = 0;

    virtual std::unique_ptr<Spell> clone() const = 0;
};

#endif

