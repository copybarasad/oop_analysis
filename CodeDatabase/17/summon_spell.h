#ifndef SUMMON_SPELL_H
#define SUMMON_SPELL_H

#include "i_summon_spell.h"
#include "game_map.h"
#include "position.h"
#include <string>

class SummonSpell : public ISummonSpell {
private:
    int range = 3;
public:
    bool cast(GameMap& map, Position casterPos, Position targetPos) override;
    std::string getName() const override { return "Призыв союзника"; }
    int getRange(int lev) const override { return range; }
};

#endif