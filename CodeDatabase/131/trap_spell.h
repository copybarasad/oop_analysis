#ifndef OOP_TRAP_SPELL_H_
#define OOP_TRAP_SPELL_H_

#include "spell.h"
#include <memory>

namespace rpg {
    class TrapSpellContext;

    class TrapSpell final : public Spell {
    public:
        static constexpr int kDefaultDamage = 25;
        static constexpr int kDefaultRange = 5;
        static constexpr int kDefaultCost = 15;

        explicit TrapSpell(int damage = kDefaultDamage,
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

        static constexpr int kEnhancedDamageBonus = 15;

        [[nodiscard]] bool IsInRange(const Position &from,
                                     const Position &to,
                                     int range) const;

        void PlaceTrap(const Position &target_pos,
                       TrapSpellContext &context,
                       int damage);
    };
}

#endif
