#include "area_damage_spell.h"
#include "spell_context.h"
#include "enemy.h"
#include "ally.h"
#include <cmath>
#include <sstream>

namespace rpg {
    AreaDamageSpell::AreaDamageSpell(int damage, int range, int area_size, int cost)
        : damage_(damage), range_(range), area_size_(area_size), cost_(cost) {
    }

    [[nodiscard]] bool AreaDamageSpell::CanCast(
        const Position &target_pos,
        const SpellContext &context) const {
        const Position player_pos = context.GetPlayerPosition();
        return IsInRange(player_pos, target_pos, range_);
    }

    void AreaDamageSpell::Cast(
        const Position &target_pos,
        SpellContext &context) {
        auto &damage_context = dynamic_cast<DamageSpellContext &>(context);
        ApplyAreaDamage(target_pos, damage_context, damage_, area_size_);
    }

    void AreaDamageSpell::CastEnhanced(
        const Position &target_pos,
        SpellContext &context) {
        auto &damage_context = dynamic_cast<DamageSpellContext &>(context);
        ApplyAreaDamage(target_pos, damage_context, damage_, area_size_ + kEnhancedAreaBonus);
    }

    void AreaDamageSpell::ApplyAreaDamage(
        const Position &target_pos,
        DamageSpellContext &context,
        const int damage,
        const int area_size) const {
        for (auto &enemy: context.GetEnemies()) {
            if (IsInArea(target_pos, enemy.GetPosition(), area_size)) {
                enemy.TakeDamage(damage);
            }
        }

        for (auto &ally: context.GetAllies()) {
            if (IsInArea(target_pos, ally.GetPosition(), area_size)) {
                ally.TakeDamage(damage / 2);
            }
        }
    }

    [[nodiscard]] std::string AreaDamageSpell::GetName() const {
        return "Облако урона";
    }

    [[nodiscard]] std::string AreaDamageSpell::GetDescription() const {
        std::ostringstream oss;
        oss << "Наносит " << damage_ << " урона в области " << area_size_ << "x" << area_size_
                << " в радиусе " << range_;
        return oss.str();
    }

    [[nodiscard]] std::string AreaDamageSpell::GetEnhancedDescription() const {
        std::ostringstream oss;
        int enhanced_area = area_size_ + kEnhancedAreaBonus;
        oss << "Наносит " << damage_ << " урона в УВЕЛИЧЕННОЙ области " << enhanced_area << "x"
                << enhanced_area << " в радиусе " << range_ << "+";
        return oss.str();
    }

    [[nodiscard]] int AreaDamageSpell::GetCost() const {
        return cost_;
    }

    [[nodiscard]] SpellType AreaDamageSpell::GetType() const {
        return SpellType::kAreaDamage;
    }

    [[nodiscard]] std::unique_ptr<Spell> AreaDamageSpell::Clone() const {
        return std::make_unique<AreaDamageSpell>(damage_, range_, area_size_, cost_);
    }

    [[nodiscard]] bool AreaDamageSpell::IsInRange(
        const Position &from,
        const Position &to,
        int range) const {
        int dx = static_cast<int>(to.x) - static_cast<int>(from.x);
        int dy = static_cast<int>(to.y) - static_cast<int>(from.y);

        double distance = std::sqrt(dx * dx + dy * dy);
        return distance <= static_cast<double>(range);
    }

    [[nodiscard]] bool AreaDamageSpell::IsInArea(
        const Position &center,
        const Position &pos,
        int area_size) const {
        int dx = std::abs(static_cast<int>(pos.x) - static_cast<int>(center.x));
        int dy = std::abs(static_cast<int>(pos.y) - static_cast<int>(center.y));

        return dx < area_size && dy < area_size;
    }
}
