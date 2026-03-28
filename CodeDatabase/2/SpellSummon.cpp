#include "SpellSummon.hpp"
#include "Ally.hpp"
#include <iostream>
#include <vector>
#include "SpellEnhancement.hpp"

SpellSummon::SpellSummon() {
    mana_cost = 5;
    range = 1;
    summon_count = 1;  // ✅ Убираем int - это инициализация, а не объявление
}

int SpellSummon::get_range() const {
    return range;
}

int SpellSummon::get_mana_cost() const {
    return mana_cost;
}

bool SpellSummon::requires_target() const {
    return false;
}

std::string SpellSummon::get_name() const {
    return "Summon Ally";
}

bool SpellSummon::cast(Player& player, Board& board, std::vector<Enemy>& enemies,
                      std::vector<Ally>& allies, EnemyAttackTower& attack_tower,
                      EnemyTower& spawn_tower, int target_x, int target_y) {

    if (player.get_mana() < mana_cost) {
        return false;
    }

    // ✅ ПРИМЕНЯЕМ УЛУЧШЕНИЯ ПЕРЕД ИСПОЛЬЗОВАНИЕМ
    int current_summon_count = 1; // базовое количество

    if (SpellEnhancement::has_enhancement()) {
        current_summon_count = SpellEnhancement::apply_summon_enhancement(current_summon_count);
        std::cout << "=== SUMMON ENHANCEMENT APPLIED ===" << std::endl;
        std::cout << "Enhanced summon count: " << current_summon_count << std::endl;
    }

    int player_x, player_y;
    player.get_coords(player_x, player_y);

    // ✅ ВСЕ 8 клеток вокруг игрока
    std::vector<std::pair<int, int>> positions = {
        {player_x - 1, player_y}, {player_x + 1, player_y},
        {player_x, player_y - 1}, {player_x, player_y + 1},
        {player_x - 1, player_y - 1}, {player_x - 1, player_y + 1},
        {player_x + 1, player_y - 1}, {player_x + 1, player_y + 1}
    };

    std::cout << "=== SUMMON DEBUG ===" << std::endl;
    std::cout << "Player at: " << player_x << ", " << player_y << std::endl;
    std::cout << "Summon count: " << current_summon_count << std::endl;

    int allies_summoned = 0;

    // ✅ Призываем несколько союзников
    for (const auto& pos : positions) {
        if (allies_summoned >= current_summon_count) break;

        int pos_x = pos.first;
        int pos_y = pos.second;

        if (pos_x >= 0 && pos_x < board.get_length() &&
            pos_y >= 0 && pos_y < board.get_width()) {

            Cell& cell = board.get_cell(pos_x, pos_y);

            if (!cell.is_obstacle_here() && !cell.is_enemy_here() &&
                !cell.is_player_here() && !cell.is_ally_here() &&
                !cell.is_tower_here() && !cell.is_attack_tower_here()) {

                allies.emplace_back(pos_x, pos_y);
                cell.set_ally(true);
                allies_summoned++;

                std::cout << "Ally " << allies_summoned << " summoned at " << pos_x << ", " << pos_y << std::endl;
            }
        }
    }

    if (allies_summoned > 0) {
        std::cout << "Successfully summoned " << allies_summoned << " allies" << std::endl;
        std::cout << "=== END SUMMON DEBUG ===" << std::endl;
        return true;
    } else {
        std::cout << "No suitable position found for summoning" << std::endl;
        std::cout << "=== END SUMMON DEBUG ===" << std::endl;
        return false;
    }
}