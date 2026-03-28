#pragma once
#include "Spell.h"


class EmpowerSpell : public Spell {
private:
    int bonus;
public:
    EmpowerSpell() : bonus(1) {};
    EmpowerSpell(int b) : bonus(b) {};
    ~EmpowerSpell() override = default;
    bool Cast(SpellContext& ctx) override;
    void enhance() override;
    std::shared_ptr<dataSpell> Save() override;
};