#include "landscape.hpp"

Landscape::Landscape(std::set<Effect> own_effects, char screen_symbol): 
    own_effects(own_effects), GameObject(screen_symbol), solidity(1e5)
{};

Landscape::Landscape(std::set<Effect> own_effects, char screen_symbol, int solidity): 
    own_effects(own_effects), GameObject(screen_symbol), solidity(solidity)
{};

std::set<Effect> Landscape::step() {
    solidity--;
    return own_effects;
}

bool Landscape::trampled() {
    return solidity <= 0;
}

GameObject *Landscape::clone() const {
    return new Landscape(*this);
}