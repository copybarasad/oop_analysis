//
// Created by Artem on 09.10.2025.
//

#ifndef LABA1_PLAYERCONTROLLER_H
#define LABA1_PLAYERCONTROLLER_H
#include <utility>

#include "../EntityManagers/CreatureManagers/PlayerManager.h"
#include "ExternalComandController/Actions/PlayerAction.h"
#include "../../Entities/Creatures/PlayerView.h"
#include "ExternalComandController/Actions/PlayerActionWithCoords.h"
#include "ExternalComandController/Actions/PlayerActionUseSpell.h"
#include "ExternalComandController/Actions/PlayerUpgradeAction.h"

class GameMasterBase;

class PlayerController {
public:
    PlayerController(const PlayerManager &manager, Field &field, const PlayerView &playerView) : manager(manager),
                                                                                                 field(field),
                                                                                                 playerView(playerView) {}

    void doMove(GameMasterBase& gameMaster, PlayerAction* action);

protected:
    PlayerManager manager;
    Field& field;
    PlayerView playerView;

    bool canMoveOn(std::vector<std::vector<int>>& distances, Constants::XYPair moveFrom, Constants::XYPair moveTo);
    void moveTo(std::vector<std::vector<int>>& distances, Constants::XYPair moveFrom, Constants::XYPair moveTo, GameMasterBase& gameMaster);
};


#endif //LABA1_PLAYERCONTROLLER_H
