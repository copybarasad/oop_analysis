#include "DamageCastResultProcessor.hpp"

DamageCastResultProcessor::DamageCastResultProcessor (EntityManager& manager, SpellHand& hand) :
    CastResultProcessor(manager, hand) {}

void DamageCastResultProcessor::processCastResult (Spell::CastResult& castResult) {
    if (castResult.success) {
        for (auto destroyed: castResult.destroyedEntities) {
            manager.removeTrack(destroyed);
        }
        hand.restoreHand();
    }
}