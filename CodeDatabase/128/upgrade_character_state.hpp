#pragma once
#include "processphase.hpp"

class UpgradeCharacterState : public ProcessPhase {
    void OnEnter() override;
    void Update(IController& controller) override;
};