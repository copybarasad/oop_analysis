#ifndef RPG_DIRECTDAMAGESPELL_H
#define RPG_DIRECTDAMAGESPELL_H

#include "Spell.h"
#include "IDirectSpell.h"

class DirectDamageSpell : public Spell, public IDirectSpell {
public:
    DirectDamageSpell(unsigned int castDiameter, unsigned int cooldown);

    explicit DirectDamageSpell(json& data);

    [[nodiscard]] std::shared_ptr<ISpell> clone() const override;

    [[nodiscard]] json to_json() const override;

    [[nodiscard]] std::string getName() const override;

    bool cast(std::pair<int,int>& coordinates, Entity* caster, GameField& field, EntityManager& entityManager) override;

    [[nodiscard]] int getCastDiameter() const override;

    bool canHitTarget(std::pair<int,int>& coordinates, Entity* caster, GameField& field, EntityManager& entityManager) override;
};

#endif