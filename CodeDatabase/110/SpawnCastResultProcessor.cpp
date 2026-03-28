#include "SpawnCastResultProcessor.hpp"

SpawnCastResultProcessor::SpawnCastResultProcessor (EntityManager& manager, SpellHand& hand) :
    CastResultProcessor(manager, hand) {}

void SpawnCastResultProcessor::processCastResult (Spell::CastResult& castResult) {
    if (castResult.success) {
        for (auto spawned: castResult.spawnedEntities) {
            manager.addTrack(spawned);
        }
        hand.restoreHand();
    }
}