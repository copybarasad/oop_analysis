#ifndef RPG_SUMMONSPELL_H
#define RPG_SUMMONSPELL_H

#include "Spell.h"
#include "IPlaceSpell.h"

class SummonSpell : public Spell, public IPlaceSpell {
public:
    SummonSpell(unsigned int castDiameter, unsigned int cooldown);

    explicit SummonSpell(json& data);

    [[nodiscard]] std::shared_ptr<ISpell> clone() const override;

    [[nodiscard]] json to_json() const override;

    [[nodiscard]] std::string getName() const override;

    bool cast(std::pair<int, int> &coordinates, Entity* caster, GameField &field, EntityManager& entityManager) override;

    [[nodiscard]] std::pair<int, int> visualData() const override;

    [[nodiscard]] bool canBePlaced(std::pair<int,int>& coordinates, GameField& field, EntityManager& entityManager) const override;
};


#endif