#ifndef SPELL
#define SPELL

#include "../../../Entities/Player/Player.hpp"
#include "../../../Field/Field/Field.hpp"
#include "../../../Field/Cell/Cell.hpp"

class Spell {
    public:
    struct CastResult {
        bool success;
        std::vector<size_t> destroyedEntities;
        std::vector<Cell*> spawnedEntities;
        size_t level;

        CastResult () : success(false), destroyedEntities({}), spawnedEntities({}), level(0) {}
    };

    virtual ~Spell() = default;

    virtual CastResult cast() = 0;
    virtual void upgradeSpell (size_t level) = 0;
};

#endif