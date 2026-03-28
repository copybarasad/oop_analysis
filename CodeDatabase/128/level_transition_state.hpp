#pragma once
#include "processphase.hpp"

class LevelTransitionState : public ProcessPhase {
    void OnEnter() override;
    void Update(IController& controller) override;
};
