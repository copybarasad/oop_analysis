#ifndef RPG_AREADAMAGESPELL_H
#define RPG_AREADAMAGESPELL_H

#include "Spell.h"
#include "IAreaSpell.h"
#include "ISpell.h"

class AreaDamageSpell : public Spell, public IAreaSpell {
    unsigned int diameter;
public:
    AreaDamageSpell(unsigned int castDiameter, unsigned int cooldown);

    explicit AreaDamageSpell(json& data);

    [[nodiscard]] std::shared_ptr<ISpell> clone() const override;

    [[nodiscard]] json to_json() const override;

    [[nodiscard]] std::string getName() const override;

    bool cast(std::pair<int, int> &coordinates, Entity* caster, GameField &field, EntityManager& entityManager) override;

    [[nodiscard]] std::pair<int, int> visualData() const override;

    [[nodiscard]] bool allCellsInRange(std::pair<int,int>& target) const override;
};


#endif