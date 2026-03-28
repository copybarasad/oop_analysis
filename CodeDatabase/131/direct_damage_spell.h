#ifndef OOP_DIRECT_DAMAGE_SPELL_H_
#define OOP_DIRECT_DAMAGE_SPELL_H_

#include "spell.h"
#include <memory>

namespace rpg {
    class DamageSpellContext;

    class DirectDamageSpell final : public Spell {
    public:
        static constexpr int kDefaultDamage = 30;
        static constexpr int kDefaultRange = 3;
        static constexpr int kDefaultCost = 20;

        explicit DirectDamageSpell(int damage = kDefaultDamage,
                                   int range = kDefaultRange,
                                   int cost = kDefaultCost);

        [[nodiscard]] bool CanCast(const Position &target_pos,
                                   const SpellContext &context) const override;

        void Cast(const Position &target_pos,
                  SpellContext &context) override;

        void CastEnhanced(const Position &target_pos,
                          SpellContext &context) override;

        [[nodiscard]] std::string GetName() const override;

        [[nodiscard]] std::string GetDescription() const override;

        [[nodiscard]] std::string GetEnhancedDescription() const override;

        [[nodiscard]] int GetCost() const override;

        [[nodiscard]] SpellType GetType() const override;

        [[nodiscard]] std::unique_ptr<Spell> Clone() const override;

    private:
        int damage_;
        int range_;
        int cost_;

        static constexpr int kEnhancedRangeBonus = 2;

        [[nodiscard]] bool IsInRange(const Position &from,
                                     const Position &to,
                                     int range) const;

        void ApplyDamage(const Position &target_pos,
                         DamageSpellContext &context,
                         int damage,
                         int range);
    };
}

#endif
