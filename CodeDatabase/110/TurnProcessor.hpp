#ifndef BASE_TURN_PROC_H
#define BASE_TURN_PROC_H

#include <algorithm>

#include "../../../Field/Field/Field.hpp"
#include "../../../Managers/EntityManager/EntityManager.hpp"
#include "../../../ChangeableFields/BaseChangeableFields/ChangebaleFields.hpp"

class TurnProcessor {
    public:
    struct TurnResult {
        bool success;
        bool playerDead;
        bool castResult;
        bool switchResult;
        bool buyResult;

        TurnResult () : success(0), playerDead(0), castResult(0), switchResult(0), buyResult(0) {}
    };

    protected:
    size_t id;
    EntityManager& manager;
    ChangeableFields* changeableFields;

    public:
    TurnProcessor (size_t id, EntityManager& manager);
    virtual ~TurnProcessor() = default;

    virtual TurnResult run () = 0;
    void setID (size_t id);
};

#endif