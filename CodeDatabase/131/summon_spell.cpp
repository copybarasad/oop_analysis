#include "summon_spell.h"
#include "spell_context.h"
#include "ally.h"
#include "enemy.h"
#include "game_field.h"
#include <sstream>
#include <vector>
#include <algorithm>

namespace rpg {
    SummonSpell::SummonSpell(int ally_health, int ally_damage, int cost)
        : ally_health_(ally_health), ally_damage_(ally_damage), cost_(cost) {
    }

    [[nodiscard]] bool SummonSpell::CanCast(
        const Position &target_pos,
        const SpellContext &context) const {
        Position player_pos = context.GetPlayerPosition();

        if (!IsAdjacentToPlayer(target_pos, player_pos)) {
            return false;
        }

        const auto &summon_context = dynamic_cast<const SummonSpellContext &>(context);

        const GameField &field = summon_context.GetField();
        if (!field.IsPassable(target_pos.x, target_pos.y)) {
            return false;
        }

        for (const auto &enemy: summon_context.GetEnemies()) {
            if (enemy.GetPosition() == target_pos) {
                return false;
            }
        }

        for (const auto &ally: summon_context.GetAllies()) {
            if (ally.GetPosition() == target_pos) {
                return false;
            }
        }

        return true;
    }

    void SummonSpell::Cast(
        const Position &target_pos,
        SpellContext &context) {
        auto &summon_context = dynamic_cast<SummonSpellContext &>(context);
        SummonAllies(target_pos, summon_context, kBaseAlliesCount);
    }

    void SummonSpell::CastEnhanced(
        const Position &target_pos,
        SpellContext &context) {
        auto &summon_context = dynamic_cast<SummonSpellContext &>(context);
        SummonAllies(target_pos, summon_context, kEnhancedAlliesCount);
    }

    void SummonSpell::SummonAllies(
        const Position &,
        SummonSpellContext &context,
        int count) {
        Position player_pos = context.GetPlayerPosition();

        std::vector<Position> adjacent_positions;

        const std::vector<std::pair<int, int> > offsets = {
            {0, -1}, {0, 1}, {-1, 0}, {1, 0}
        };

        for (const auto &[dx, dy]: offsets) {
            int new_x = static_cast<int>(player_pos.x) + dx;
            int new_y = static_cast<int>(player_pos.y) + dy;

            if (new_x >= 0 && new_y >= 0) {
                Position pos(static_cast<size_t>(new_x), static_cast<size_t>(new_y));

                if (context.GetField().IsPassable(pos.x, pos.y)) {
                    bool occupied = false;

                    for (const auto &enemy: context.GetEnemies()) {
                        if (enemy.GetPosition() == pos) {
                            occupied = true;
                            break;
                        }
                    }

                    for (const auto &ally: context.GetAllies()) {
                        if (ally.GetPosition() == pos) {
                            occupied = true;
                            break;
                        }
                    }

                    if (!occupied) {
                        adjacent_positions.push_back(pos);
                    }
                }
            }
        }

        int spawned = 0;
        for (const auto &pos: adjacent_positions) {
            if (spawned >= count) {
                break;
            }
            context.GetAllies().emplace_back(ally_health_, ally_damage_, pos);
            ++spawned;
        }
    }

    [[nodiscard]] std::string SummonSpell::GetName() const {
        return "Призыв союзника";
    }

    [[nodiscard]] std::string SummonSpell::GetDescription() const {
        std::ostringstream oss;
        oss << "Призывает " << kBaseAlliesCount << " союзника (" << ally_health_ << " ОЗ, "
                << ally_damage_ << " урона) рядом с игроком";
        return oss.str();
    }

    [[nodiscard]] std::string SummonSpell::GetEnhancedDescription() const {
        std::ostringstream oss;
        oss << "Призывает " << kEnhancedAlliesCount << " союзников (" << ally_health_ << " ОЗ, "
                << ally_damage_ << " урона) рядом с игроком+";
        return oss.str();
    }

    [[nodiscard]] int SummonSpell::GetCost() const {
        return cost_;
    }

    [[nodiscard]] SpellType SummonSpell::GetType() const {
        return SpellType::kSummon;
    }

    [[nodiscard]] std::unique_ptr<Spell> SummonSpell::Clone() const {
        return std::make_unique<SummonSpell>(ally_health_, ally_damage_, cost_);
    }

    [[nodiscard]] bool SummonSpell::IsAdjacentToPlayer(
        const Position &pos,
        const Position &player_pos) const {
        int dx = std::abs(static_cast<int>(pos.x) - static_cast<int>(player_pos.x));
        int dy = std::abs(static_cast<int>(pos.y) - static_cast<int>(player_pos.y));

        return (dx <= 1 && dy <= 1 && (dx + dy > 0));
    }
}
