#ifndef COMMONSPELL_H
#define COMMONSPELL_H
#include "Spell.h"

class CommonSpell : public Spell {
protected:
    std::string name;
    int damage;
    int range;
    bool isAOE;

public:
    CommonSpell(const std::string& name, int damage, int range, bool isAOE);
    virtual ~CommonSpell() = default;

    std::string getName() const override { return name; }
    int getDamage() const override { return damage; }
    int getRange() const override { return range; }
    bool getIsAOE() const override { return isAOE; }

    void setName(const std::string& name) { this->name = name; }
    void setDamage(int damage) { this->damage = damage; }
    void setRange(int range) { this->range = range; }
    void setIsAOE(bool isAOE) { this->isAOE = isAOE; }
};



#endif //COMMONSPELL_H
