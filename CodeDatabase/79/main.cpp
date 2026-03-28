#include <iostream>
#include <string>
#include <vector>
#include "basic_info.h"
#include "player.h"
#include "enemy.h"
#include "tower.h"
#include "movement.h"
#include "place.h"
#include "cell.h"
#include "imagic_spell_card.h"
#include "area_spell.h"
#include "default_spell.h"
#include "spell_hand.h"
#include "trap_spell.h"
#include "magic_tower.h"
#include "weak_default_spell.h"
#include "game.h"
#include "commands.h"
#include "console_input.h"
#include "game_controller.h"
#include "render.h"
#include "render_controller.h"

int main() {
    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}