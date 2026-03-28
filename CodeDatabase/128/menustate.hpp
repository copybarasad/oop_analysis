#pragma once
#include "processphase.hpp"

class MenuState : public ProcessPhase {
    void OnEnter() override;
    void Update(IController& controller) override;
};