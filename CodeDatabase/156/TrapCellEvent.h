//
// Created by Artem on 24.10.2025.
//

#ifndef LABAOOP2_TRAPCELLEVENT_H
#define LABAOOP2_TRAPCELLEVENT_H

#include <map>
#include "CellEvent.h"

class TrapCellEvent : public CellEvent{
public:
    TrapCellEvent(int damage): CellEvent(CellEventType::TrapEvent), damage(damage){}

    void impactOnCreatureInCell(Creature &creature) override;

    std::unique_ptr<CellEvent> clone() const override;

    std::string serialize() override;
    static TrapCellEvent* deserialize(std::map<std::string, std::string> fields) noexcept;
private:
    int damage;
};


#endif //LABAOOP2_TRAPCELLEVENT_H
