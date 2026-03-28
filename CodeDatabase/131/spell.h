#ifndef OOP_SPELL_H_
#define OOP_SPELL_H_

#include <string>
#include <memory>
#include "position.h"

namespace rpg {
    class SpellContext;

    enum class SpellType {
        kDirectDamage,
        kAreaDamage,
        kTrap,
        kSummon,
        kEnhancement
    };

    class Spell {
    public:
        virtual ~Spell() = default;

        [[nodiscard]] virtual bool CanCast(const Position &target_pos,
                                           const SpellContext &context) const = 0;

        virtual void Cast(const Position &target_pos,
                          SpellContext &context) = 0;

        virtual void CastEnhanced(const Position &target_pos,
                                  SpellContext &context) {
            Cast(target_pos, context);
        }

        [[nodiscard]] virtual std::string GetName() const = 0;

        [[nodiscard]] virtual std::string GetDescription() const = 0;

        [[nodiscard]] virtual std::string GetEnhancedDescription() const {
            return GetDescription() + "+";
        }

        [[nodiscard]] virtual int GetCost() const = 0;

        [[nodiscard]] virtual SpellType GetType() const = 0;

        [[nodiscard]] virtual std::unique_ptr<Spell> Clone() const = 0;
    };
}

#endif
