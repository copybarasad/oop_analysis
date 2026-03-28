#pragma once
// Одноразовые баффы для следующего каста.
struct UpgradeState {
    int rangeBonus{0};
    int areaBonus{0};
    int trapDamageBonus{0};
    int summonCountBonus{0};
    int stacks{0};
    void reset() { rangeBonus = areaBonus = trapDamageBonus = summonCountBonus = stacks = 0; }
};
