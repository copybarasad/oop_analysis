#ifndef OOP_SUMMON_SPELL_H_
#define OOP_SUMMON_SPELL_H_

#include "spell.h"
#include <memory>

namespace rpg {
    class SummonSpellContext;

    class SummonSpell final : public Spell {
    public:
        static constexpr int kDefaultAllyHealth = 40;
        static constexpr int kDefaultAllyDamage = 15;
        static constexpr int kDefaultCost = 40;

        explicit SummonSpell(int ally_health = kDefaultAllyHealth,
                             int ally_damage = kDefaultAllyDamage,
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
        int ally_health_;
        int ally_damage_;
        int cost_;

        static constexpr int kBaseAlliesCount = 1;
        static constexpr int kEnhancedAlliesCount = 2;

        [[nodiscard]] bool IsAdjacentToPlayer(const Position &pos,
                                              const Position &player_pos) const;

        void SummonAllies(const Position &target_pos,
                          SummonSpellContext &context,
                          int count);
    };
}

#endif
