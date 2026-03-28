#ifndef RPG_UPGRADESPELL_H
#define RPG_UPGRADESPELL_H

#include "Spell.h"
#include <iostream>

class UpgradeSpell : public Spell {
    int target;
public:
    explicit UpgradeSpell(unsigned int cooldown);

    explicit UpgradeSpell(json& data);

    [[nodiscard]] std::shared_ptr<ISpell> clone() const override;

    [[nodiscard]] json to_json() const override;

    [[nodiscard]] std::string getName() const override;

    bool cast(std::pair<int,int>& coordinates, Entity* caster, GameField& field, EntityManager& entityManager) override;

    void setTarget(int index);

    [[nodiscard]] std::pair<int,int> visualData() const override;
};

#endif
