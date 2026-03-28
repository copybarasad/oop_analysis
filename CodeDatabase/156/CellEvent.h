//
// Created by Artem on 24.10.2025.
//

#ifndef LABAOOP2_CELLEVENT_H
#define LABAOOP2_CELLEVENT_H

#include "CellEventType.h"
#include "../../Entities/Creatures/Creature.h"
class CellEvent {
public:
    explicit CellEvent(CellEventType type) : type(type) {}

    virtual CellEventType getType() const noexcept final;
    virtual ~CellEvent() = default;
    virtual std::unique_ptr<CellEvent> clone() const = 0;
    virtual void impactOnCreatureInCell(Creature& creature) = 0;
    virtual std::string serialize() = 0;
private:
    CellEventType type;
};


#endif //LABAOOP2_CELLEVENT_H
