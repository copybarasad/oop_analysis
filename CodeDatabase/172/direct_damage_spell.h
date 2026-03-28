#pragma once

#include <string>

#include "ispell.h"
#include "spell_context.h"
#include "pos.h"

namespace game {
    class ETower;

    class DirectDamageSpell final : public ISpell {
        private:
            int baseDamage_;
            int radius_;
        
        public:
            DirectDamageSpell(int dmg, int r)
                : baseDamage_(dmg), radius_(r) {}
            
                std::string name() const override { return "Direct Damage"; }

                bool canCast(const SpellContext& ctx) const override;

                bool cast(SpellContext& ctx) override;

                void applyLevelUp(const GameConfig& cfg) override;

                // Special method for Enemy Tower
                bool castFromTower(Field& field, const ETower& tower);

                int getDamage()     const override { return baseDamage_; }
                int getRadius()     const override { return radius_; }

                void setDamage(int dmg) override { baseDamage_ = dmg; }
                void setRadius(int r)   override { radius_ = r; }

                std::unique_ptr<ISpell> clone() const override {
                    return std::make_unique<DirectDamageSpell>(baseDamage_, radius_);
                }

                SpellId id() const noexcept override { return SpellId::DirectDamage; }
    };
}