#ifndef OOP_SPELL_CONTEXT_H_
#define OOP_SPELL_CONTEXT_H_

#include <vector>
#include "position.h"

namespace rpg {
    class GameField;
    class Enemy;
    class EnemyBuilding;
    class Ally;
    class Trap;

    class SpellContext {
    public:
        virtual ~SpellContext() = default;

        [[nodiscard]] virtual Position GetPlayerPosition() const = 0;
        [[nodiscard]] virtual const GameField& GetField() const = 0;
    };

    class DamageSpellContext : public virtual SpellContext {
    public:
        [[nodiscard]] virtual std::vector<Enemy>& GetEnemies() = 0;
        [[nodiscard]] virtual std::vector<EnemyBuilding>& GetBuildings() = 0;
        [[nodiscard]] virtual std::vector<Ally>& GetAllies() = 0;
    };

    class TrapSpellContext : public virtual SpellContext {
    public:
        [[nodiscard]] virtual std::vector<Trap>& GetTraps() = 0;
        [[nodiscard]] virtual std::vector<Enemy>& GetEnemies() = 0;
        [[nodiscard]] virtual std::vector<Ally>& GetAllies() = 0;
        [[nodiscard]] virtual std::vector<EnemyBuilding>& GetBuildings() = 0;
        [[nodiscard]] virtual const std::vector<Trap>& GetTraps() const = 0;
        [[nodiscard]] virtual const std::vector<Enemy>& GetEnemies() const = 0;
        [[nodiscard]] virtual const std::vector<Ally>& GetAllies() const = 0;
    };

    class SummonSpellContext : public virtual SpellContext {
    public:
        [[nodiscard]] virtual std::vector<Ally>& GetAllies() = 0;
        [[nodiscard]] virtual const std::vector<Ally>& GetAllies() const = 0;
        [[nodiscard]] virtual const std::vector<Enemy>& GetEnemies() const = 0;
        [[nodiscard]] virtual const std::vector<EnemyBuilding>& GetBuildings() const = 0;
    };

    class EnhancementSpellContext : public virtual SpellContext {
    public:
        virtual void ActivateEnhancement() = 0;
    };
}

#endif

