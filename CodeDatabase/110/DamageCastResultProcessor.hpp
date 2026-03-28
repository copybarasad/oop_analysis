#ifndef DAMAGE_CAST_PROC_H
#define DAMAGE_CAST_PROC_H

#include "../BaseCastResultProcessor/CastResultProcessor.hpp"

class DamageCastResultProcessor : public CastResultProcessor {
    public:
    DamageCastResultProcessor (EntityManager& manager, SpellHand& hand);
    void processCastResult (Spell::CastResult& castResult) override;
};

#endif