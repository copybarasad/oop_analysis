//
// Created by Artem on 17.12.2025.
//

#ifndef LABAOOP2_PLAYERACTION_H
#define LABAOOP2_PLAYERACTION_H

#include "ActionType.h"

class PlayerAction {
public:
    explicit PlayerAction(ActionType type) : type(type) {}
    ActionType type;
};


#endif //LABAOOP2_PLAYERACTION_H
