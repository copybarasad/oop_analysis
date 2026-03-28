#ifndef LABAOOP2_PLAYERUPGRADEACTION_H
#define LABAOOP2_PLAYERUPGRADEACTION_H

#include "PlayerAction.h"
#include "UpgradesType.h"

class PlayerUpgradeAction : public PlayerAction{
public:
    PlayerUpgradeAction(ActionType type, UpgradesType upgradeType) : PlayerAction(type), upgradeType(upgradeType) {}
    UpgradesType upgradeType;
};

#endif //LABAOOP2_PLAYERUPGRADEACTION_H