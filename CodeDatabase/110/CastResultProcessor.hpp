#ifndef BASE_CAST_PROC_H
#define BASE_CAST_PROC_H

#include "../../../../Managers/EntityManager/EntityManager.hpp"
#include "../../../../Spells/SpellHand/SpellHand.hpp"
#include "../../../../Spells/Spell/BaseSpell/Spell.hpp"

class CastResultProcessor {
    protected:
    EntityManager& manager;
    SpellHand& hand;

    public:
    CastResultProcessor (EntityManager& manager, SpellHand& hand);
    virtual ~CastResultProcessor () = default;

    virtual void processCastResult (Spell::CastResult& castResult) = 0;
};

#endif