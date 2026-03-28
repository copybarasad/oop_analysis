#ifndef TRAPS_H
#define TRAPS_H

#include "spell.h"
#include "entity.h"
#include "cell.h"
class GameField;

class Traps : public ISpell {
private:
    int dmg;
    int castX;
    int castY;
    std::string name;
    SpellType id;

public:
    Traps(int dmg = 8, std::string name = "TrapSpell", SpellType id = TRAP);
    void cast(const CastContext &context) const override;
    int getDamage() const { return dmg; }
    std::string getName() const override;
    void upgrade() override;
    SpellType getTypeId() const override {return id;}
    int getDmg() const override {return dmg;}
    void setDmg(int dmg) override {this->dmg = dmg;}
};

#endif