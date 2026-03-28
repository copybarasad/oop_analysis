#ifndef OOP_ENHANCEMENT_SPELL_H_
#define OOP_ENHANCEMENT_SPELL_H_

#include "spell.h"
#include <memory>

namespace rpg {
    class EnhancementSpellContext;

    class EnhancementSpell final : public Spell {
    public:
        static constexpr int kDefaultCost = 10;

        explicit EnhancementSpell(int cost = kDefaultCost);

        [[nodiscard]] bool CanCast(const Position &target_pos,
                                   const SpellContext &context) const override;

        void Cast(const Position &target_pos,
                  SpellContext &context) override;

        [[nodiscard]] std::string GetName() const override;

        [[nodiscard]] std::string GetDescription() const override;

        [[nodiscard]] int GetCost() const override;

        [[nodiscard]] SpellType GetType() const override;

        [[nodiscard]] std::unique_ptr<Spell> Clone() const override;

    private:
        int cost_;
    };
}

#endif
