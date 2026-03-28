//
// Created by Artem on 17.12.2025.
//

#ifndef LABAOOP2_PLAYERVIEW_H
#define LABAOOP2_PLAYERVIEW_H

#include "Player.h"

class PlayerView {
public:
    explicit PlayerView(Player &player) : player(player) {}

    const CloseRangeAttack &getCloseRangeAttack() noexcept;

    const LongRangeAttack &getLongRangeAttack() noexcept;

    int getXCoordinate() noexcept;

    int getYCoordinate() noexcept;

    int getHealthPoints() noexcept;

    bool isAlive() noexcept;

    bool isDisabled() noexcept;

    int getStepRange() noexcept;

    long getScore() noexcept;

    const SpellHand &getSpellHand() noexcept;

    bool isCloseRangeAttackSelected() noexcept;

private:
    Player &player;
};


#endif //LABAOOP2_PLAYERVIEW_H
