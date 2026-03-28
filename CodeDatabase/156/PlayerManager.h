//
// Created by Artem on 02.10.2025.
//

#ifndef LABA1_PLAYERMANAGER_H
#define LABA1_PLAYERMANAGER_H
#include "CreatureManager.h"
#include "../../../Entities/Creatures/Player.h"
#include "../../Controllers/ExternalComandController/Actions/UpgradesType.h"

class PlayerManager : public CreatureManager{
public:
    PlayerManager(Field &field, std::shared_ptr<Player>& player) : CreatureManager(field, player.get()),
                                                                   player(player) {}
    void moveTo(Constants::dxdy withOffset) override;
    void attack(Constants::XYPair coords, AttackType type);
    void changeAttackType() noexcept;
    SpellHand& getSpellHand() noexcept;
    void upgradePlayer(UpgradesType type);

protected:
    std::shared_ptr<Player> player;
};


#endif //LABA1_PLAYERMANAGER_H
