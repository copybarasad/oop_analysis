#pragma once
#include "Item.hpp"
#include "../core/Config.hpp"
#include "../core/SaveData.hpp"

class SpellScroll : public Item {
protected:
    int powerOfSpell;
    virtual bool cast(GameContext& context, int userIndex, int power) {
        (void)context;
        (void)userIndex;
        (void)power;
        return false;
    };
public:
    SpellScroll(int power) { powerOfSpell = power; }
    SpellScroll() : SpellScroll(1) {}
    virtual ~SpellScroll() = default;
    void upgrade() {
        powerOfSpell++;
    }
    void useItem(GameContext& context, int userIndex, int power) override {
        if (cast(context, userIndex, power)) {
            this->decCountOfItem();
        }
    }
    virtual SpellSaveData getSpellSaveData() = 0;
};