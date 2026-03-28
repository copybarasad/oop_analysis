#ifndef SPAWN_CAST_PROC_H
#define SPAWN_CAST_PROC_H

#include "../BaseCastResultProcessor/CastResultProcessor.hpp"

class SpawnCastResultProcessor : public CastResultProcessor {
    public:
    SpawnCastResultProcessor (EntityManager& manager, SpellHand& hand);
    void processCastResult (Spell::CastResult& castResult) override;
};

#endif