#pragma once
#include "spell.hpp"
#include "constants.hpp"

using namespace configs;

class SpellParams {
public:
    SpellParams() = default;

    static SpellParams Default(SpellType type) {
        SpellParams p;
        switch (type) {
            case SpellType::kDirectDamage: p.direct_radius_ = kDirectRadius; break;
            case SpellType::kAreaDamage:   p.area_size_     = kAreaSide;     break;
            case SpellType::kTrapped:      p.trap_damage_   = kTrapDamage;   break;
            case SpellType::kSummon:       p.summon_count_  = kSummonCount;  break;
            default: break;
        }
        return p;
    }
    int getDirectRadius() const noexcept { return direct_radius_; }
    int getAreaSize() const noexcept { return area_size_; }
    int getTrapDamage() const noexcept { return trap_damage_; }
    int getSummonCount() const noexcept { return summon_count_; }

    void SetDirectRadius(int v) noexcept { direct_radius_ = v; }
    void SetAreaSize(int v) noexcept { area_size_ = v; }
    void SetTrapDamage(int v) noexcept { trap_damage_ = v; }
    void SetSummonCount(int v) noexcept { summon_count_ = v; }

private:
    int direct_radius_ = 0;
    int area_size_     = 0;
    int trap_damage_   = 0;
    int summon_count_  = 0;
};

