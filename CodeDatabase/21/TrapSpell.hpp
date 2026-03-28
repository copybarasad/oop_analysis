#pragma once
#include "ISpell.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"
#include "../core/states/TrapEffect.hpp"

class TrapSpell : public SpellScroll {
private:
    int trapLevel;
    int trapDamage;
public:
    bool cast(GameContext& ctx, int userIndex, int power) override;
    TrapSpell() : TrapSpell(1, 50){};
    TrapSpell(int trapLevel, int trapDamage) : trapLevel(trapLevel), trapDamage(trapDamage) {};
    TrapSpell(const SpellSaveData& data) : trapLevel(data.trapLevel), trapDamage(data.trapDamage) {
        this->powerOfSpell = data.powerOfSpell;
        this->countOfItem = data.countOfItem;
    }
    SpellSaveData getSpellSaveData();
};
