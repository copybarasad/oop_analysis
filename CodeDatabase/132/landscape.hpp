#pragma once

#include "game_object.hpp"
#include "ILandscape.hpp"

class Landscape: public GameObject, public ILandscape {
protected:
    std::set<Effect> own_effects;
    int solidity;
public:
    Landscape(std::set<Effect> own_effects, char screen_symbol, int solidity);
    Landscape(std::set<Effect> own_effects, char screen_symbol);
    std::set<Effect> step() override;
    bool trampled() override;
    GameObject* clone() const override;
};