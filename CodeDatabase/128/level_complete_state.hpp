#pragma once
#include "processphase.hpp"

class LevelCompleteState : public ProcessPhase {
    void OnEnter() override;
    void Update(IController& controller) override;
};