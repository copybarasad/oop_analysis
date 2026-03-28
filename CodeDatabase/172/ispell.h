#pragma once

#include <string>
#include <memory>

namespace game {
    enum class SpellId { 
        AreaDamage   = 0,
        DirectDamage = 1,
        Trap         = 2,
    };

    struct SpellContext;
    struct GameConfig;
    class ISpell {
        public:
            virtual ~ISpell() = default;

            virtual std::string name() const = 0;

            virtual bool canCast(const SpellContext& ctx) const = 0;

            virtual bool cast(SpellContext& ctx) = 0;
            
            virtual void applyLevelUp(const GameConfig& cfg) = 0;

            virtual int getDamage() const = 0;
            virtual int getRadius() const = 0;

            virtual void setDamage(int dmg) = 0;
            virtual void setRadius(int dmg) = 0;

            virtual std::unique_ptr<ISpell> clone() const = 0;

            virtual SpellId id() const noexcept = 0;
    };
}