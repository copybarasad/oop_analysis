//
// Created by Artem on 24.10.2025.
//

#ifndef LABAOOP2_SLOWINGCELLEVENT_H
#define LABAOOP2_SLOWINGCELLEVENT_H

#include "CellEvent.h"
#include <map>
#include <string>

class SlowingCellEvent : public CellEvent{
public:
    explicit SlowingCellEvent() : CellEvent(CellEventType::SlowingEvent) {}
    void impactOnCreatureInCell(Creature &creature) override;

    std::unique_ptr<CellEvent> clone() const override;

    std::string serialize() override;
    static SlowingCellEvent* deserialize() noexcept;
private:

};


#endif //LABAOOP2_SLOWINGCELLEVENT_H
