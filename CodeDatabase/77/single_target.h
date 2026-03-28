#ifndef SINGLE_TARGET_H
#define SINGLE_TARGET_H
#include "spell.h"
#include <vector>

class SingleTarget : public ISpell {
private:
    int dmg;
    int radius;
    std::string name;
    SpellType id;
public:
    SingleTarget(int dmg = 15, int radius = 5, std::string name = "SingleTargetSpell", SpellType id = SINGLE);
    void cast(const CastContext &context) const override;
    std::string getName() const override;
    void upgrade() override;
    SpellType getTypeId() const override {return id;}
    int getDmg() const override {return dmg;}
    void setDmg(int dmg) override {this->dmg = dmg;}
};  

#endif