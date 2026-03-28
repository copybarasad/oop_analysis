#include "trap_spell.h"
#include "player.h"
#include "field.h"
#include <iostream>



TrapSpell::TrapSpell(std::string name, int damage) :
    name(std::move(name)), damage(damage) {}


std::string TrapSpell::get_name() const {
    return name;
}


// каст - устанавливает ловушку по позиции игрока на поле (меняется тип клетки поля)
void TrapSpell::cast(Player& caster, GameState& game_state) {
    std::cout << "Casting " << name << "!\n";

    Position trap_pos = caster.get_position();

    if (game_state.get_field().get_cell_at(trap_pos).get_type() != CellType::TRAP) {
        game_state.get_field().set_or_remove_trap(trap_pos, damage);
        std::cout << "Trap set at (" << trap_pos.get_x() << "," << trap_pos.get_y()
                  << ") with " << damage << " damage!\n";
    } else {
        std::cout << "Cannot set Bear Trap here! It already has a trap!\n";

    }

}