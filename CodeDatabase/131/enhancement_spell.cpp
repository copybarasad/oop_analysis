#include "enhancement_spell.h"
#include "spell_context.h"

namespace rpg {
    EnhancementSpell::EnhancementSpell(int cost) : cost_(cost) {
    }

    [[nodiscard]] bool EnhancementSpell::CanCast(
        const Position &,
        const SpellContext &) const {
        return true;
    }

    void EnhancementSpell::Cast(
        const Position &,
        SpellContext &context) {
        auto &enhancement_context = dynamic_cast<EnhancementSpellContext &>(context);
        enhancement_context.ActivateEnhancement();
    }

    [[nodiscard]] std::string EnhancementSpell::GetName() const {
        return "Усиление";
    }

    [[nodiscard]] std::string EnhancementSpell::GetDescription() const {
        return "Усиливает ваше СЛЕДУЮЩЕЕ заклинание (на один ход)";
    }

    [[nodiscard]] int EnhancementSpell::GetCost() const {
        return cost_;
    }

    [[nodiscard]] SpellType EnhancementSpell::GetType() const {
        return SpellType::kEnhancement;
    }

    [[nodiscard]] std::unique_ptr<Spell> EnhancementSpell::Clone() const {
        return std::make_unique<EnhancementSpell>(cost_);
    }
}
