#pragma once
#include "icontroller.hpp" 

class ProcessPhase {
public:
    virtual ~ProcessPhase() = default;

    virtual void OnEnter() = 0;
    virtual  void Update(IController& controller) = 0;
};