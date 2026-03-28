#pragma once

#include <string>

#include "ispell.h"

namespace game {
    class TrapSpell final : public ISpell {
        private:
            int dmg_;
        
        public:
            TrapSpell(int power) : dmg_(power) {}

            std::string name() const override { return "Trap"; }

            bool canCast(const SpellContext& ctx) const override;
            bool cast(SpellContext& ctx) override;

            void applyLevelUp(const GameConfig& cfg) override;

            int getDamage() const override { return dmg_; }
            int getRadius() const override { return 0; }

            void setDamage(int dmg) override { dmg_ = dmg; }
            void setRadius(int r)   override { (void)r; }

            std::unique_ptr<ISpell> clone() const override {
                return std::make_unique<TrapSpell>(dmg_);
            }
            
            SpellId id() const noexcept override { return SpellId::Trap; }
    };
}