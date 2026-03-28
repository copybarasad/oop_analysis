#ifndef DAMAGE_SPELL_H
#define DAMAGE_SPELL_H

#include "spell_card.h"
#include <iostream>
#include <cstdint>

class GameController;

class DamageSpell : public SpellCard {
public:
    DamageSpell(const std::string& name,
        const std::string& description,
        int damage,
        int range,
        GameController* gameController);

    virtual ~DamageSpell() = default;

    // ќбща¤ реализаци¤ методов
    bool cast() override;
    std::string getName() const override;
    std::string getDescription() const override;
    bool canCast() const override;

    void setTarget(const Position& target) override;
    bool isValidTarget(const Position& target) const override;
    std::vector<Position> getValidTargets() const override;
    int getRange() const override;
    int getDamage() const override;

    virtual void save(std::ostream& out) const = 0;
    virtual void load(std::istream& in) = 0;

protected:
    std::string spellName;
    std::string spellDescription;
    int spellDamage;
    int spellRange;
    GameController* gameController;
    Position targetPosition;
    virtual bool applyEffect() = 0;
};

#endif


