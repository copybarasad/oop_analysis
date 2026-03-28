#ifndef CAST_S_H
#define CAST_S_H

#include <map>

#include "../../../Spells/SpellHand/SpellHand.hpp"
#include "../../../Field/Field/Field.hpp"
#include "../../../Factories/SpellFactories/BaseSpellFactory/SpellFactory.hpp"
#include "../CastResultProcessor/BaseCastResultProcessor/CastResultProcessor.hpp"
#include "../../../ChangeableFields/CastChangeableFields/CastChangeableFields/CastChangeableFields.hpp"

class CastService {
    private:
    std::map<std::string, SpellFactory*> factories;
    std::map<std::string, CastResultProcessor*> processors;

    public:
    CastService() = default;
    ~CastService ();
    
    void registerFactory (const std::string& name, SpellFactory* factory);
    void registerProcessor (const std::string& name, CastResultProcessor* processor);
    bool castSpell (ChangeableFields& changeableFields);
};

#endif