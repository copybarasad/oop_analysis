#pragma once
#include "Spell.h"

class BuffSpell : public Spell {
public:
    enum class BuffType {
        DAMAGE,
        AREA,
        TRAP,
        SUMMON
    };

private:
    BuffType type_;

public:
    explicit BuffSpell(BuffType type = BuffType::DAMAGE);
    std::string getName() const override;
    bool apply(Field& field, const Player& player, int targetX, int targetY) override;
    BuffType getBuffType() const { return type_; }
    bool isBuff() const override { return true; }
    bool needsTarget() const override { return false; }
};