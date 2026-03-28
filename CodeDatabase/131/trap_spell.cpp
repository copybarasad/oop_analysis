#include "trap_spell.h"
#include "spell_context.h"
#include "trap.h"
#include "enemy.h"
#include "ally.h"
#include "game_field.h"
#include <cmath>
#include <sstream>

namespace rpg {
    TrapSpell::TrapSpell(int damage, int range, int cost)
        : damage_(damage), range_(range), cost_(cost) {
    }

    [[nodiscard]] bool TrapSpell::CanCast(
        const Position &target_pos,
        const SpellContext &context) const {
        Position player_pos = context.GetPlayerPosition();

        if (!IsInRange(player_pos, target_pos, range_)) {
            return false;
        }

        const auto &trap_context = dynamic_cast<const TrapSpellContext &>(context);

        const GameField &field = trap_context.GetField();
        const Cell &cell = field.GetCell(target_pos.x, target_pos.y);

        if (cell.GetType() != CellType::kNormal) {
            return false;
        }

        for (const auto &trap: trap_context.GetTraps()) {
            if (trap.GetPosition() == target_pos) {
                return false;
            }
        }

        for (const auto &enemy: trap_context.GetEnemies()) {
            if (enemy.GetPosition() == target_pos) {
                return false;
            }
        }

        for (const auto &ally: trap_context.GetAllies()) {
            if (ally.GetPosition() == target_pos) {
                return false;
            }
        }

        if (player_pos == target_pos) {
            return false;
        }

        return true;
    }

    void TrapSpell::Cast(
        const Position &target_pos,
        SpellContext &context) {
        auto &trap_context = dynamic_cast<TrapSpellContext &>(context);
        PlaceTrap(target_pos, trap_context, damage_);
    }

    void TrapSpell::CastEnhanced(
        const Position &target_pos,
        SpellContext &context) {
        auto &trap_context = dynamic_cast<TrapSpellContext &>(context);
        PlaceTrap(target_pos, trap_context, damage_ + kEnhancedDamageBonus);
    }

    void TrapSpell::PlaceTrap(
        const Position &target_pos,
        TrapSpellContext &context,
        int damage) {
        context.GetTraps().emplace_back(target_pos, damage);
    }

    [[nodiscard]] std::string TrapSpell::GetName() const {
        return "Ловушка";
    }

    [[nodiscard]] std::string TrapSpell::GetDescription() const {
        std::ostringstream oss;
        oss << "Размещает ловушку, наносящую " << damage_ << " урона в радиусе " << range_;
        return oss.str();
    }

    [[nodiscard]] std::string TrapSpell::GetEnhancedDescription() const {
        std::ostringstream oss;
        oss << "Размещает МОЩНУЮ ловушку, наносящую " << (damage_ + kEnhancedDamageBonus)
                << " урона в радиусе " << range_ << "+";
        return oss.str();
    }

    [[nodiscard]] int TrapSpell::GetCost() const {
        return cost_;
    }

    [[nodiscard]] SpellType TrapSpell::GetType() const {
        return SpellType::kTrap;
    }

    [[nodiscard]] std::unique_ptr<Spell> TrapSpell::Clone() const {
        return std::make_unique<TrapSpell>(damage_, range_, cost_);
    }

    [[nodiscard]] bool TrapSpell::IsInRange(
        const Position &from,
        const Position &to,
        int range) const {
        int dx = static_cast<int>(to.x) - static_cast<int>(from.x);
        int dy = static_cast<int>(to.y) - static_cast<int>(from.y);

        double distance = std::sqrt(dx * dx + dy * dy);
        return distance <= static_cast<double>(range);
    }
}
