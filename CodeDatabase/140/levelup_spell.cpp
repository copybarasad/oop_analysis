#include "levelup_spell.h"
#include "field.h"
#include "player.h"
#include <iostream>

void LevelupSpell::use(Field& field, int caster_x, int caster_y, int target_x, int target_y) {
    Player* player = field.get_player();
    if (!player) return;

    player->add_level_up();
    cout << "Leveled up! Level: " << player->get_level() << "\n";
}