#ifndef OOP_AREA_DAMAGE_SPELL_H_
#define OOP_AREA_DAMAGE_SPELL_H_

#include "spell.h"
#include <memory>

namespace rpg {
    class DamageSpellContext;

    class AreaDamageSpell final : public Spell {
    public:
        static constexpr int kDefaultDamage = 20;
        static constexpr int kDefaultRange = 4;
        static constexpr int kDefaultAreaSize = 2;
        static constexpr int kDefaultCost = 30;

        explicit AreaDamageSpell(int damage = kDefaultDamage,
                                 int range = kDefaultRange,
                                 int area_size = kDefaultAreaSize,
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
        int area_size_;
        int cost_;

        static constexpr int kEnhancedAreaBonus = 1;

        [[nodiscard]] bool IsInRange(const Position &from,
                                     const Position &to,
                                     int range) const;

        [[nodiscard]] bool IsInArea(const Position &center,
                                    const Position &pos,
                                    int area_size) const;

        void ApplyAreaDamage(const Position &target_pos,
                             DamageSpellContext &context,
                             int damage,
                             int area_size) const;
    };
}

#endif
