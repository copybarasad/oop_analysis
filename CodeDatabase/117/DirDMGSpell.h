#ifndef DIRDMGSPELL_H
#define DIRDMGSPELL_H

#include "ISpell.h"
#include "../Constants.h"

class DirDMGSpell: public ISpell{
private:
    int damage;
    int range;
    int cost;
    Character& caster;
    Character& target;

public:
    DirDMGSpell(int damage, int range, Character& caster, Character& target);
    DirDMGSpell(Character& caster, Character& target);
    bool cast(GameState& gameState) override;
    void setDMG(int damage);
    int getDMG() const;
    void setRange(int range);
    int getRange() const;
    void upgrade(int power);
    std::string getName() const override;
    int getCost() const override;
};

#endif