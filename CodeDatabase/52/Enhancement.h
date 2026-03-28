#pragma once
struct Enhancement {
    int radiusBonus = 0;
    int areaBonus = 0;
    int trapDamageBonus = 0;
    int summonBonus = 0;
    int stacks = 0;

    void Reset() noexcept {
        radiusBonus = areaBonus = trapDamageBonus = summonBonus = stacks = 0;
    }
    bool Active() const noexcept { return stacks > 0; }
};