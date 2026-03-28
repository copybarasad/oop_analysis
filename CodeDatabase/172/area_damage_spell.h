#pragma once

#include <string>

#include "ispell.h"

namespace game {
    class AreaDamageSpell final : public ISpell {
        private:
            int baseDamage_;
            int radius_;
        
        public:
            AreaDamageSpell(int dmg, int r = 2)
                : baseDamage_(dmg), radius_(r) {}
            
            std::string name() const override { return "Area Damage"; }

            bool canCast(const SpellContext& ctx) const override;
            bool cast(SpellContext& ctx) override;

            void applyLevelUp(const GameConfig& cfg) override;

            int getDamage()     const override { return baseDamage_; }
            int getRadius()     const override { return radius_; }

            void setDamage(int dmg)    override { baseDamage_ = dmg; }
            void setRadius(int r)      override { radius_ = r; }

            std::unique_ptr<ISpell> clone() const override { 
                return std::make_unique<AreaDamageSpell>(baseDamage_, radius_);
            }

            SpellId id() const noexcept override { return SpellId::AreaDamage; }
    };
}