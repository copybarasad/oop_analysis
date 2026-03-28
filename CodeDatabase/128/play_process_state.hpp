#pragma once
#include "processphase.hpp"

class PlayProcessState : public ProcessPhase {
    void OnEnter() override;
    void Update(IController& controller) override;
};