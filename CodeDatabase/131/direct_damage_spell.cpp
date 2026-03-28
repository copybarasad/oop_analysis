#include "direct_damage_spell.h"
#include "spell_context.h"
#include "enemy.h"
#include "enemy_building.h"
#include <cmath>
#include <sstream>

namespace rpg {
    DirectDamageSpell::DirectDamageSpell(int damage, int range, int cost)
        : damage_(damage), range_(range), cost_(cost) {
    }

    [[nodiscard]] bool DirectDamageSpell::CanCast(
        const Position &target_pos,
        const SpellContext &context) const {
        const Position player_pos = context.GetPlayerPosition();

        return IsInRange(player_pos, target_pos, range_);
    }

    void DirectDamageSpell::Cast(
        const Position &target_pos,
        SpellContext &context) {
        auto &damage_context = dynamic_cast<DamageSpellContext &>(context);
        ApplyDamage(target_pos, damage_context, damage_, range_);
    }

    void DirectDamageSpell::CastEnhanced(
        const Position &target_pos,
        SpellContext &context) {
        auto &damage_context = dynamic_cast<DamageSpellContext &>(context);
        ApplyDamage(target_pos, damage_context, damage_, range_ + kEnhancedRangeBonus);
    }

    void DirectDamageSpell::ApplyDamage(
        const Position &target_pos,
        DamageSpellContext &context,
        int damage,
        int) {
        for (auto &enemy: context.GetEnemies()) {
            if (enemy.GetPosition() == target_pos) {
                enemy.TakeDamage(damage);
                return;
            }
        }

        for (auto &building: context.GetBuildings()) {
            if (building.GetPosition() == target_pos) {
                return;
            }
        }
    }

    [[nodiscard]] std::string DirectDamageSpell::GetName() const {
        return "Прямой урон";
    }

    [[nodiscard]] std::string DirectDamageSpell::GetDescription() const {
        std::ostringstream oss;
        oss << "Наносит " << damage_ << " урона одной цели в радиусе " << range_;
        return oss.str();
    }

    [[nodiscard]] std::string DirectDamageSpell::GetEnhancedDescription() const {
        std::ostringstream oss;
        oss << "Наносит " << damage_ << " урона в РАСШИРЕННОМ радиусе "
                << (range_ + kEnhancedRangeBonus) << "+";
        return oss.str();
    }

    [[nodiscard]] int DirectDamageSpell::GetCost() const {
        return cost_;
    }

    [[nodiscard]] SpellType DirectDamageSpell::GetType() const {
        return SpellType::kDirectDamage;
    }

    [[nodiscard]] std::unique_ptr<Spell> DirectDamageSpell::Clone() const {
        return std::make_unique<DirectDamageSpell>(damage_, range_, cost_);
    }

    [[nodiscard]] bool DirectDamageSpell::IsInRange(
        const Position &from,
        const Position &to,
        int range) const {
        int dx = static_cast<int>(to.x) - static_cast<int>(from.x);
        int dy = static_cast<int>(to.y) - static_cast<int>(from.y);

        double distance = std::sqrt(dx * dx + dy * dy);
        return distance <= static_cast<double>(range);
    }
}
