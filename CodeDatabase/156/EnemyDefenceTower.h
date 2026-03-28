//
// Created by Artem on 30.10.2025.
//

#ifndef LABAOOP2_ENEMYDEFENCETOWER_H
#define LABAOOP2_ENEMYDEFENCETOWER_H

#include <utility>

#include "Building.h"
#include "../Creatures/Attacks/Spels/DirectDamageSpell.h"
#include "../../Factories/SpellFactory.h"

class EnemyDefenceTower : public Building{
public:
    EnemyDefenceTower(int xCoordinate, int yCoordinate, int healthPoint, int attackRadius, int attackInterval) : Building(xCoordinate, yCoordinate, healthPoint, EntityType::EnemyDefenceTower),
                                                                                              attackRadius(attackRadius),
                                                                                              attackInterval(attackInterval) {
        spell = DirectDamageSpell();
    }

    virtual const DirectDamageSpell& getDamageSpell() const noexcept final;
    virtual int getAttackRadius() const noexcept final;
    virtual int getAttackInterval() const noexcept final;

    std::unique_ptr<Entity> clone() const override;

    std::string serialize() override;
    static EnemyDefenceTower* deserialize(std::map<std::string, std::string> json) noexcept;

protected:
    DirectDamageSpell spell;
    int attackRadius, attackInterval;
};


#endif //LABAOOP2_ENEMYDEFENCETOWER_H
