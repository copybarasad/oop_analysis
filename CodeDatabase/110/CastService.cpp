#include "CastService.hpp"

CastService::~CastService () {
    for (auto& pair : factories) {
        delete pair.second;
    }
    for (auto& pair : processors) {
        delete pair.second;
    }
}

void CastService::registerFactory (const std::string& name, SpellFactory* factory) {
    factories[name] = factory;
}

void CastService::registerProcessor (const std::string& name, CastResultProcessor* processor) {
    processors[name] = processor;
}

bool CastService::castSpell (ChangeableFields& changeableFields) {
    CastFields& cf = dynamic_cast<CastFields&>(changeableFields);

    Spell* castingSpell = factories[cf.getSpellName()]->createSpell(changeableFields);
    auto castResult = castingSpell->cast();
    if (processors.find(cf.getSpellName()) != processors.end()) {
        processors[cf.getSpellName()]->processCastResult(castResult);
    }
    delete castingSpell;
    return castResult.success;
}