#ifndef I_LEVEL_UP_H
#define I_LEVEL_UP_H

#include "ispell.h"

class ILevelUpSpell : public ISpell {

public:
    bool cast(GameMap& map, Position casterPos, Position target) override;
    std::string getName() const override { return "Улучшение уровня"; }
    int getRange(int lev) const override { return 1; }
};

#endif