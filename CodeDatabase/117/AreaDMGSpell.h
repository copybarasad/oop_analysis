#ifndef AREADMGSPELL_H
#define AREADMGSPELL_H

#include "ISpell.h"
#include "../Constants.h"

class AreaDMGSpell: public ISpell{
private:
    int damage;
    int range;
    int lenSide;
    int cost;
    Character& caster;
    std::pair<int, int> pos;

public:
    AreaDMGSpell(Character& caster, std::pair<int, int> pos);
    void setLen(int len);
    int getLen() const;
    void setDMG(int damage);
    int getDMG() const;
    void setRange(int range);
    int getRange() const;

    void upgrade(int power) override;
    bool cast(GameState& gameState) override;
    std::string getName() const override;
    int getCost() const override;
};

#endif