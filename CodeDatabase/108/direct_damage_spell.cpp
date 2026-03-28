#include "direct_damage_spell.h"

#include "player.h"
#include "game_state.h"
#include <iostream>



DirectDamageSpell::DirectDamageSpell(std::string  name, int damage, int range, AttackDirection dir) :
    name(std::move(name)), damage(damage), range(range), direction(dir) {}


std::string DirectDamageSpell::get_name() const {
    return name;
}


// ИСПРАВЛЕНО после 2 лб. Нет трех полностью идентичных огромных for
void DirectDamageSpell::cast(Player& caster, GameState& game_state) {
    std::cout << "Casting " << name << "!\n";

    if (direction == AttackDirection::TOWER_TO_PLAYER) {     // случай: башня бьет игрока (ослаблено)
        int initial_health = caster.get_health();
        caster.take_damage(damage);

        std::cout << "Hit player for " << damage << " damage! ";
        std::cout << "Player health: " << initial_health << " -> " << caster.get_health() << "\n";

        if (!caster.is_alive()) {
            std::cout << "Player defeated!\n";
        }

    } else {   // случай игрок бьет противника

        // ищем ближайшую цель (алгоритм вынесен в game_state и унифицирован через сведение к Entity и лямбда функцию)
        Entity* nearest_target = game_state.find_nearest_target(caster.get_position(), range);

        // Бьем ближайшую цель + вывод справочной информации
        if (nearest_target) {
            int initial_health = nearest_target->get_health();
            nearest_target->take_damage(damage);

            std::cout << "Hit " << nearest_target->get_name() << " for " << damage << " damage! ";
            std::cout << "Health: " << initial_health << " -> " << nearest_target->get_health() << "\n";

            if (!nearest_target->is_alive()) {
                std::cout << nearest_target->get_name() << " destroyed!\n";
            }
        } else {
            std::cout << "target wasn't found\n";
        }
    }

}