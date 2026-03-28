//
// Created by Artem on 17.12.2025.
//

#include "PlayerView.h"

const CloseRangeAttack &PlayerView::getCloseRangeAttack() noexcept {
    return this->player.getCloseRangeAttack();
}

const LongRangeAttack &PlayerView::getLongRangeAttack() noexcept {
    return this->player.getLongRangeAttack();
}

int PlayerView::getXCoordinate() noexcept {
    return this->player.getXCoordinate();
}

int PlayerView::getYCoordinate() noexcept {
    return this->player.getYCoordinate();
}

int PlayerView::getHealthPoints() noexcept {
    return this->player.getHealthPoints();
}

bool PlayerView::isAlive() noexcept {
    return this->player.isAlive();
}

bool PlayerView::isDisabled() noexcept {
    return this->player.isDisabled();
}

int PlayerView::getStepRange() noexcept {
    return this->player.getStepRange();
}

long PlayerView::getScore() noexcept {
    return this->player.getScore();
}

const SpellHand &PlayerView::getSpellHand() noexcept {
    return this->player.getSpellHand();
}

bool PlayerView::isCloseRangeAttackSelected() noexcept {
    return this->player.isCloseRangeAttackSelected();
}
