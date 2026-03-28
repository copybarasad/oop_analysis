//
// Created by Artem on 17.12.2025.
//

#ifndef LABAOOP2_PLAYERACTIONUSESPELL_H
#define LABAOOP2_PLAYERACTIONUSESPELL_H

#include "PlayerActionWithCoords.h"

class PlayerActionUseSpell : public PlayerActionWithCoords {
public:
    PlayerActionUseSpell(ActionType type, const Constants::XYPair &xy, int spellNum) : PlayerActionWithCoords(type, xy),
                                                                                       spellNum(spellNum) {}

    int spellNum;
};


#endif //LABAOOP2_PLAYERACTIONUSESPELL_H
