#pragma once
#include "ISpell.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"
#include "../core/states/AttackEffect.hpp"
#include "../core/states/IState.hpp"
#include "../core/SaveData.hpp"

class AreaDamageSpell : public SpellScroll {
private:
    int baseDamage;
    int baseDistance;
public:
    AreaDamageSpell() : AreaDamageSpell(6, 4, 1, 0) {}
    AreaDamageSpell(int damage, int distance, int power, int count) {
        this->baseDamage = damage;
        this->baseDistance = distance;
        this->powerOfSpell = power;
        this->countOfItem = count;
    }
    AreaDamageSpell(const SpellSaveData& data) : baseDamage(data.baseDamage), baseDistance(data.baseDistance) {
        this->powerOfSpell = data.powerOfSpell;
        this->countOfItem = data.countOfItem;
    }
    ~AreaDamageSpell() override = default;
    bool cast(GameContext& ctx, int userIndex, int power) override;
    SpellSaveData getSpellSaveData();
private:
    void animateCast(GameContext& ctx, int userIndex, int distance);
};
