//
// Created by Artem on 17.12.2025.
//

#ifndef LABAOOP2_PLAYERACTIONWITHCOORDS_H
#define LABAOOP2_PLAYERACTIONWITHCOORDS_H

#include "PlayerAction.h"
#include "../../../../Constants/Constants.h"

class PlayerActionWithCoords : public PlayerAction {
public:
    PlayerActionWithCoords(ActionType type, const Constants::XYPair &xy) : PlayerAction(type), xy(xy) {}

    Constants::XYPair xy;
};


#endif //LABAOOP2_PLAYERACTIONWITHCOORDS_H
