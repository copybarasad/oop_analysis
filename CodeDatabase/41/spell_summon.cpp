#include "spell_summon.h"
#include "player.h"
#include "game_field.h"
#include "ally.h"
#include "constants.h"
#include <iostream>
#include <vector>
#include <cmath>

Spell_summon::Spell_summon(int count)
    : Spell_card("Призыв союзника", "Призывает союзника для помощи в бою", SPELL_SUMMON_COST, SPELL_SUMMON_RANGE),
    summon_count(count){
}

bool Spell_summon::can_cast(const Player& player, int target_x, int target_y, const Game_field& field) const{
    (void)player;     
    (void)target_x;
    (void)target_y;
    (void)field;
    return true;
}

void Spell_summon::cast(Player& player, int target_x, int target_y, Game_field& field) {
    (void)target_x;  
    (void)target_y;
    std::cout << player.get_name() << " призывает союзников!\n";
    auto player_pos = field.get_player_position();
    int player_x = player_pos.first;
    int player_y = player_pos.second;
    
    std::vector<std::pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    int summoned = 0;
    for (const auto& dir : directions) {
        if (summoned >= summon_count) break;
        int spawn_x = player_x + dir.first;
        int spawn_y = player_y + dir.second;
        if (field.is_valid_position(spawn_x, spawn_y) && 
            field.get_cell(spawn_x, spawn_y).can_move_to()) {
            Ally* new_ally = new Ally("Союзник", &field);
            field.add_ally(new_ally, spawn_x, spawn_y);
            std::cout << "Призван союзник в позиции (" << spawn_x << ", " << spawn_y << ")\n";
            summoned++;
        }
    } 
    if (summoned == 0) {
        std::cout << "Нет свободных клеток для призыва союзников!\n";
    } else {
        std::cout << "Успешно призвано союзников: " << summoned << "\n";
    }
}

Spell_card* Spell_summon::clone() const {
    return new Spell_summon(*this);
}

void Spell_summon::enhance() {
    summon_count++;
    enhanced = true;
    std::cout << name << " усилено! Теперь призывает " << summon_count << " союзников\n";
}
